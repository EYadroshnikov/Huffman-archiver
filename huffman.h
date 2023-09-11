#ifndef HUFFMAN_HEADER
#define HUFFMAN_HEADER

#include <stdbool.h>

#include "common.h"

typedef unsigned char uchar;

typedef struct huffman_node_t huffman_node_t;

struct huffman_node_t {
	uchar symbol;
	int count;
	huffman_node_t *left_child;
	huffman_node_t *right_child;
};

huffman_node_t *create_child(uchar symbol, int count) {
	huffman_node_t *child = safe_malloc(sizeof(huffman_node_t));
	child->symbol = symbol;
	child->count = count;
	child->left_child = NULL;
	child->right_child = NULL;
	return child;
}

huffman_node_t *create_parent(
	huffman_node_t *left_child,
	huffman_node_t *right_child
) {
	huffman_node_t *parent = safe_malloc(sizeof(huffman_node_t));
	parent->count = left_child->count + right_child->count;
	parent->left_child = left_child;
	parent->right_child = right_child;
	return parent;
}

bool is_leaf(huffman_node_t *node) {
	return node->left_child == NULL && node->right_child == NULL;
}

void swap_nodes(huffman_node_t **a, huffman_node_t **b) {
	huffman_node_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

void clear_tree(huffman_node_t *node){
	if(is_leaf(node)){
		clear_tree(node->left_child);
		clear_tree(node->right_child);
	}
	free(node);
}

#endif
