#ifndef BINHEAP_HEADER
#define BINHEAP_HEADER

#include <stdio.h>

#include "common.h"
#include "huffman.h"

typedef struct heap_t {
	int size;
	int max_size;
	huffman_node_t **arr;
} heap_t;

heap_t* init(int size) {
	heap_t* heap = safe_malloc(sizeof(heap_t));
	heap->size = 0;
	heap->max_size = size;
	heap->arr = safe_malloc(size * sizeof(huffman_node_t *));
	return heap;
}

void add(heap_t *heap, huffman_node_t *value) {
	if (heap->size == heap->max_size) {
		printf("heap overflow\n");
		return;
	}
	heap->arr[heap->size] = value;
	heap->size++;
	int child_index = heap->size - 1;
	int parent_index = (child_index - 1) / 2;
	while (child_index != 0 && heap->arr[parent_index]->count > heap->arr[child_index]->count) {
		swap_nodes(&heap->arr[parent_index], &heap->arr[child_index]);
		child_index = parent_index;
		parent_index = (child_index - 1) / 2;
	}
}

huffman_node_t *extract_min(heap_t *heap) {
	if (heap->size == 0) {
		printf("heap is empty");
		exit(EXIT_FAILURE);
	}
	huffman_node_t *to_return = heap->arr[0];
	heap->arr[0] = heap->arr[--heap->size];

	int parent_index = 0;
	int left_child_index = 1;
	int right_child_index = 2;
	while (left_child_index < heap->size || right_child_index < heap->size) {
		int min_child_index = left_child_index;
		if (right_child_index < heap->size && heap->arr[right_child_index]->count < heap->arr[left_child_index]->count) {
			min_child_index = right_child_index;
		}

		if (heap->arr[parent_index]->count > heap->arr[min_child_index]->count) {
			swap_nodes(&heap->arr[parent_index], &heap->arr[min_child_index]);
		} else {
			break;
		}

		parent_index = min_child_index;
		left_child_index = parent_index * 2 + 1;
		right_child_index = parent_index * 2 + 2;
	}
	return to_return;
}

void print(heap_t* heap) {
	for (int i = 0; i < heap->size; ++i) {
		printf("(%c %d) ", heap->arr[i]->symbol, heap->arr[i]->count);
	}
	printf("\n");
}

#endif
