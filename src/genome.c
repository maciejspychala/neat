#include <stdlib.h>
#include <stdbool.h>
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes) {
    static struct List *global_genes = NULL;
    if(!global_genes) {
        global_genes = new_list();
    }

    struct Genome *genome = calloc(1, sizeof(struct Genome));
    genome->nodes = new_list();
    genome->global_genes = global_genes;

    for (uint32_t i = 0; i < input_nodes; i++) {
        add_data(genome->nodes, new_node(NULL, IN));
    }

    for (uint32_t i = 0; i < output_nodes; i++) {
        struct Node *out = new_node(new_list(), OUT);

        struct ListItem *walk = genome->nodes->head;
        while (walk) {
            if (((struct Node*) walk->data)->type == IN) {
                add_data(out->in_genes, new_gene(genome, ((struct Node*) walk->data)->id, out->id, random_weight()));
            }
            walk = walk->next;
        }
        add_data(genome->nodes, out);
    }
    return genome;
}

void calculate_output(struct Genome *genome, float *input) {
    struct ListItem *walk = genome->nodes->head;
    struct List *progress = new_list();
    while (walk) {
        struct Node *node = walk->data;
        node->visited = false;
        // to keep recurent connection working
        // node->value = 0.0;
        if (node->type == IN) {
            node->value = input[node->id - 1];
            node->visited = true;
        }
        if (node->type == OUT) {
            push_data(progress, node);
        }
        walk = walk->next;
    }


    while (progress->head) {
        struct Node *node = progress->head->data;

        if (node->visited) {
            float value = 0.0;
            struct ListItem *walk = node->in_genes->head;
            while (walk) {
                struct Gene *gene = walk->data;
                if (gene->enabled) {
                    value += gene->weight * find_node(genome, gene->from)->value;
                }
                walk = walk->next;
            }
            node->value = value;
            pop_data(progress);
        } else {
            node->visited = true;
            struct ListItem *walk = node->in_genes->head;
            while (walk) {
                struct Gene *gene = walk->data;
                struct Node *in_node = find_node(genome, gene->from);
                if (!in_node->visited) {
                    push_data(progress, in_node);
                }
                walk = walk->next;
            }
        }
    }
}

struct Node* find_node(struct Genome *genome, uint32_t id) {
    struct ListItem *walk = genome->nodes->head;
    while(walk) {
        if (((struct Node*) walk->data)->id == id) {
            return walk->data;
        }
        walk = walk->next;
    }
    return NULL;
}

struct Node* find_or_create_node(struct Genome *genome, uint32_t id) {
    struct Node *node = find_node(genome, id);
    if (!node) {
        node = new_node_with_id(id);
        add_data(genome->nodes, node);
    }
    return node;
}

void evolve_gene(struct Genome *genome, uint32_t in_id, uint32_t out_id) {
    struct Node *out = find_node(genome, out_id);
    struct Gene *gene = find_gene(out, in_id);
    gene->enabled = false;

    struct Node *inter = new_node(new_list(), HIDDEN);
    add_data(inter->in_genes, new_gene(genome, in_id, inter->id, 1.0));
    add_data(out->in_genes, new_gene(genome, inter->id, out->id, gene->weight));
    add_data(genome->nodes, inter);
}

void evolve_genes_weights(struct Genome *genome) {
    struct ListItem *walk = genome->nodes->head;
    while (walk) {
        struct Node *node = walk->data;
        if (node->type != IN) {
            struct ListItem *gene_walk = node->in_genes->head;
            while (gene_walk) {
                evolve_weight(gene_walk->data);
                gene_walk = gene_walk->next;
            }
        }
        walk = walk->next;
    }
}

void print_genome(struct Genome *genome) {
    struct ListItem *node = genome->nodes->head;
    while (node) {
        print_node((struct Node*) node->data);
        node = node->next;
    }
}

struct Gene* global_gene_exists(struct Genome *genome, uint32_t in, uint32_t out) {
    struct ListItem *walk = genome->global_genes->head;
    while (walk) {
        struct Gene *gene = walk->data;
        if (gene->from == in && gene->to == out) {
            return gene;
        }
        walk = walk->next;
    }
    return NULL;
}

void add_gene(struct Genome *genome, uint32_t in, uint32_t out, float weight) {
    struct Node *out_node = find_or_create_node(genome, out);
    if (find_gene(out_node, in)) {
        return;
    }
    find_or_create_node(genome, in);
    struct Gene *global_gene = global_gene_exists(genome, in, out);
    struct Gene *gene = NULL;
    if (global_gene) {
        gene = calloc(1, sizeof(struct Gene));
        *gene = *global_gene;
        gene->weight = weight;
        add_data(out_node->in_genes, gene);
    } else {
        gene = new_gene(genome, in, out, weight);
        add_data(out_node->in_genes, gene);
    }
}

struct Genome* copy_genome(struct Genome *genome) {
    struct Genome *new = calloc(1, sizeof(struct Genome));
    new->global_genes = genome->global_genes;
    new->nodes = new_list();
    struct ListItem *walk = genome->nodes->head;
    while (walk) {
        struct Node *node = walk->data;
        struct Node *new_node = calloc(1, sizeof(struct Node));
        new_node->id = node->id;
        new_node->type = node->type;
        new_node->in_genes = copy_list(node->in_genes, sizeof(struct Gene));
        add_data(new->nodes, new_node);
        walk = walk->next;
    }
    return new;
}
