#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <stdio.h>
#include <stdlib.h>

void safe_read_int(int *addr) {
	if (scanf("%d", addr) == 0) {
		printf("bad input\n");
		exit(EXIT_FAILURE);
	}
}

void *safe_malloc(size_t size) {
	void *allocated = malloc(size);
	if (!allocated) {
		printf("memory not allocated\n");
		exit(EXIT_FAILURE);
	}
	return allocated;
}

FILE *safe_fopen(char *pathname, char *mode) {
	FILE *file = fopen(pathname, mode);
	if (file == NULL) {
		printf("can't open file '%s'\n", pathname);
		exit(EXIT_FAILURE);
	}
	return file;
}

void swap(int *a, int *b) {
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

#endif
