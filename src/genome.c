#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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
    genome->input_nodes = input_nodes;
    genome->output_nodes = output_nodes;

    for (uint32_t i = 0; i < input_nodes; i++) {
        add_data(genome->nodes, new_node(new_list(), IN));
    }

    add_data(genome->nodes, new_node(new_list(), BIAS));

    for (uint32_t i = 0; i < output_nodes; i++) {
        struct Node *out = new_node(new_list(), OUT);
        void create_gene(void *data) {
            struct Node *node = data;
            if (node->type == IN || node->type == BIAS) {
                add_data(out->in_genes, new_gene(genome, node->id, out->id, random_weight()));
            }
        }
        iterate_list(genome->nodes, create_gene);
        add_data(genome->nodes, out);
    }
    return genome;
}

double sigmoid(double x) {
    return 2.0 / (1.0 + exp(-4.9 * x)) - 1;
}

void calculate_output(struct Genome *genome, double *input) {
    struct List *progress = new_list();

    void init_values(void *node_data) {
        struct Node *node = node_data;
        node->visited = false;
        if (node->type == IN) {
            node->value = input[node->id - 1];
            node->visited = true;
        } else if (node->type == BIAS) {
            node->value = 1;
            node->visited = true;
        } else if (node->type == OUT) {
            push_data(progress, node);
        }
    }
    iterate_list(genome->nodes, init_values);

    while (progress->head) {
        struct Node *node = progress->head->data;

        if (node->visited) {
            calculate_node(genome, node);
            pop_data(progress);
        } else {
            node->visited = true;
            void add_unvisited_nodes(void *gene_data) {
                struct Gene *gene = gene_data;
                struct Node *in_node = find_node(genome, gene->from);
                if (!in_node->visited) {
                    push_data(progress, in_node);
                }
            }
            iterate_list(node->in_genes, add_unvisited_nodes);
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

    void evolve_gene_weight(void* gene_data) {
        struct Gene *gene = gene_data;
        if (rand() % 5 == 0) {
            evolve_weight(gene);
        }
    }

    void evolve_node_genes_weights(void *node_data) {
        struct Node *node = node_data;
        if (node->type != IN) {
            iterate_list(node->in_genes, evolve_gene_weight);
        }
    }

    iterate_list(genome->nodes, evolve_node_genes_weights);
}

void print_genome(struct Genome *genome) {
    void print(void *node_data) {
        struct Node *node = node_data;
        print_node(node);
    }
    iterate_list(genome->nodes, print);
    printf("score: %f\n", genome->fitness);
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

void add_gene(struct Genome *genome, uint32_t in, uint32_t out, double weight) {
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
    new->input_nodes = genome->input_nodes;
    new->output_nodes = genome->output_nodes;
    new->nodes = new_list();

    void copy_node(void *node_data) {
        struct Node *node = node_data;
        struct Node *new_node = calloc(1, sizeof(struct Node));
        new_node->id = node->id;
        new_node->type = node->type;
        new_node->in_genes = copy_list(node->in_genes, sizeof(struct Gene));
        add_data(new->nodes, new_node);
    }
    iterate_list(genome->nodes, copy_node);

    return new;
}

struct Genome* crossover(struct Genome *better, struct Genome *worse) {
    struct Genome *new = calloc(1, sizeof(struct Genome));
    new->nodes = new_list();
    new->global_genes = better->global_genes;
    new->input_nodes = better->input_nodes;
    new->output_nodes = better->output_nodes;

    if (better->fitness < worse->fitness) {
        struct Genome *tmp = worse;
        worse = better;
        better = tmp;
    }

    struct ListItem *walk = better->nodes->head;

    while (walk) {
        struct Node *better_node = walk->data;
        struct Node *worse_node = find_node(worse, better_node->id);
        struct Node *new_node = new_node_with_id(better_node->id);
        new_node->type = better_node->type;

        void inherit_gene(void *gene_data) {
            struct Gene *better_gene = gene_data;
            struct Gene *worse_gene = worse_node ? find_gene(worse_node, better_gene->from) : NULL;
            struct Gene *new_gene = calloc(1, sizeof(struct Gene));

            if (worse_gene && rand() % 2)  {
                *new_gene = *worse_gene;
            } else {
                *new_gene = *better_gene;
            }
            add_data(new_node->in_genes, new_gene);
        }
        iterate_list(better_node->in_genes, inherit_gene);

        add_data(new->nodes, new_node);
        walk = walk->next;
    }

    return new;
}

double* collect_output(struct Genome *genome, uint32_t outputs) {
    double *out = calloc(outputs, sizeof(double));
    uint32_t collected = 0;
    void get_output(void *node_data) {
        struct Node *node = node_data;
        if (node->type == OUT) {
            out[collected] = node->value;
            collected++;
        }
    }
    iterate_list(genome->nodes, get_output);
    return out;
}

double distance(struct Genome *g1, struct Genome *g2) {
    if (list_size(g1->nodes) < list_size(g2->nodes)) {
        struct Genome *tmp = g2;
        g2 = g1;
        g1 = tmp;
    }
    struct ListItem *walk = g1->nodes->head;
    double disjonts = 0;
    double weight_dif = 0;
    double matching_genes = 0;

    while (walk) {
        struct Node *n1 = walk->data;
        struct Node *n2 = find_node(g2, n1->id);

        void gene_distance (void *gene_data) {
            struct Gene *g1 = gene_data;
            struct Gene *g2 = n2 ? find_gene(n2, g1->from) : NULL;

            if (g2)  {
                matching_genes++;
                weight_dif += fabs(g1->weight - g2->weight);
            } else {
                disjonts++;
            }
        }
        iterate_list(n1->in_genes, gene_distance);

        walk = walk->next;
    }

    return (disjonts / list_size(g1->nodes)) + (0.4 * weight_dif / matching_genes);
}

void destroy_genome(struct Genome *genome) {
    struct ListItem *walk = genome->nodes->head;
    while (walk) {
        struct Node *node = walk->data;
        destroy_list(node->in_genes);
        free(walk->data);
        struct ListItem *tmp = walk;
        walk = walk->next;
        free(tmp);
    }
    free(genome);
    genome = NULL;
}

struct Genome* child_add_connection(struct Genome *genome) {
    uint32_t from = rand() % list_size(genome->nodes);
    uint32_t to = (rand() % (list_size(genome->nodes) - genome->input_nodes - 1)) +
        genome->input_nodes + 1;
    struct Node *in_node = get_item(genome->nodes, from)->data;
    struct Node *out_node = get_item(genome->nodes, to)->data;
    struct Genome *new = copy_genome(genome);
    add_gene(new, in_node->id, out_node->id, random_weight());
    return new;
}

struct Genome* child_add_node(struct Genome *genome) {
    uint32_t id = (rand() % (list_size(genome->nodes) - genome->input_nodes - 1)) +
        genome->input_nodes + 1;
    struct Node *node = get_item(genome->nodes, id)->data;
    struct Gene *gene = get_item(node->in_genes, rand() % list_size(node->in_genes))->data;
    struct Genome *new = copy_genome(genome);
    evolve_gene(new, gene->from, gene->to);
    return new;
}
