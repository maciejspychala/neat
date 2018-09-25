#include <stdlib.h>
#include "gene.h"
#include "genome.h"
#include "node.h"

struct Genome* new_genome(uint32_t input_nodes, uint32_t output_nodes) {
    struct Genome *genome = calloc(1, sizeof(struct Genome));
    genome->nodes = new_list();

    for (uint32_t i = 0; i < input_nodes; i++) {
        add_data(genome->nodes, new_node(NULL, IN));
    }

    for (uint32_t i = 0; i < output_nodes; i++) {
        struct Node *out = new_node(new_list(), OUT);

        struct ListItem *walk = genome->nodes->head;
        while (walk) {
            if (((struct Node*) walk->data)->type == IN) {
                add_data(out->in_genes, new_gene(((struct Node*) walk->data)->id, out->id, random_weight()));
            }
            walk = walk->next;
        }
        add_data(genome->nodes, out);
    }
    return genome;
}

float calculate_output(struct Genome* genome, float* input, uint32_t size) {
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
            pop_item(progress);
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

struct Node* find_node(struct Genome* genome, uint32_t node) {
    struct ListItem *walk = genome->nodes->head;
    while(walk) {
        if (((struct Node*) walk->data)->id == node) {
            return walk->data;
        }
        walk = walk->next;
    }
    return NULL;
}

void evolve_gene(struct Genome* genome, uint32_t in_id, uint32_t out_id) {
    struct Node *out = find_node(genome, out_id);
    struct Gene *gene = find_gene(out, in_id);
    gene->enabled = false;

    struct Node *inter = new_node(new_list(), HIDDEN);
    add_data(inter->in_genes, new_gene(in_id, inter->id, 1.0));
    add_data(out->in_genes, new_gene(inter->id, out->id, gene->weight));
    add_data(genome->nodes, inter);
}
