#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binheap.h"
#include "common.h"
#include "huffman.h"

typedef unsigned char uchar;

const int BUF_SIZE = 100;
const int ALPHABET_SIZE = 256;
const int CHAR_SIZE_IN_BITS = 8;

void dfs(huffman_node_t *node, uchar *cur_code, uchar **dict) {
	if (node == NULL) {
		return;
	}

	if (is_leaf(node)) {
		dict[node->symbol] = safe_malloc((CHAR_SIZE_IN_BITS + 1) * sizeof(uchar));
		strcpy(dict[node->symbol], cur_code);
		return;
	}

	int code_size = strlen(cur_code);

	cur_code[code_size] = '1';
	cur_code[code_size + 1] = '\0';
	dfs(node->left_child, cur_code, dict);

	cur_code[code_size] = '0';
	dfs(node->right_child, cur_code, dict);

	cur_code[code_size] = '\0';
}

uchar code_to_uchar(uchar *code){
	uchar to_return = 0;
	for(int i = 0; i < CHAR_SIZE_IN_BITS && code[i] != '\0'; i++){
		if(code[i] == '1'){
			to_return |= 1<<(CHAR_SIZE_IN_BITS-i-1);
		}
	}
	return to_return;
}

void uchar_to_code(uchar symbol, uchar *code){
	// for(int i = 7; i >= 0; i--){
	// 	code[i] = symbol%2 == 1 ? '1' : '0';
	// 	symbol/=2;
	// }
	for(int i = 0; i < CHAR_SIZE_IN_BITS; i++){
		code[i] = (symbol >> (CHAR_SIZE_IN_BITS-i-1)) & 1 ? '1' : '0';
	}
	code[8] = '\0';
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("input file name not found in arguments\n");
		return EXIT_FAILURE;
	}

	if (argc < 3) {
		printf("output file name not found in arguments\n");
		return EXIT_FAILURE;
	}

	FILE *in_file = safe_fopen(argv[1], "rb");
	uchar buf[BUF_SIZE];
	int bytes_read = fread(&buf[0], sizeof(uchar), BUF_SIZE, in_file);
	printf("%s\n", buf);
	//printf("%u\n", bytes_read);

	int frequences[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		frequences[i] = 0;
	}

	for (int i = 0; i < bytes_read; ++i) {
		frequences[buf[i]]++;
	}

	heap_t *heap = init(ALPHABET_SIZE);
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		if (frequences[i] > 0) {
			huffman_node_t *new_node = create_child(i, frequences[i]);
			add(heap, new_node);
		}
	}

	//print(heap);

	while (heap->size > 1) {
		huffman_node_t *first_min = extract_min(heap);
		huffman_node_t *second_min = extract_min(heap);
		huffman_node_t *new_node = create_parent(first_min, second_min);
		add(heap, new_node);

		//print(heap);
	}

	huffman_node_t *root = extract_min(heap);
	free(heap);

	uchar code[CHAR_SIZE_IN_BITS + 1];
	uchar *dict[ALPHABET_SIZE];

	for(int i = 0; i < ALPHABET_SIZE; i++){
		dict[i] = NULL;
	}

	code[0] = '\0';
	dfs(root, code, dict);

	//print dict
	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(dict[i] != NULL){
			printf("%c: %s\n", i, dict[i]);
		}
	}

	uchar *encoded_text = safe_malloc((CHAR_SIZE_IN_BITS * bytes_read + 1) * sizeof(uchar));
	uchar *string_end = encoded_text;

	for(int i = 0; i < bytes_read; i++){
		strcpy(string_end, dict[buf[i]]);
		string_end += strlen(dict[buf[i]]);
	}
	printf("%s\n", encoded_text);


	FILE *out_file = safe_fopen(argv[2], "wb");
	int code_size = strlen(encoded_text);
	int bits_in_last_byte = 8;
	// char bb[1];
	// bb[0] = (char)bits_in_last_byte;
	// fwrite(bb, sizeof(char), strlen(bb), out_file);
	for(int i = 0, j = 0; i < code_size; i+=8, j++){
		if(code_size - i < 8){
			bits_in_last_byte = code_size - i;
		}
		encoded_text[j] = code_to_uchar(&encoded_text[i]);
		encoded_text[j + 1] = '\0';
	}

	printf("%s\n", encoded_text);

	fwrite(encoded_text, sizeof(uchar), strlen(encoded_text), out_file);

	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(dict[i] != NULL){
			free(dict[i]);
		}
	}

	uchar encoded_symbols[strlen(encoded_text) * CHAR_SIZE_IN_BITS + 1];
	for(int i = 0, j = 0; i < strlen(encoded_text); i++, j+=8){
		uchar_to_code(encoded_text[i], &encoded_symbols[j]);
	}

	printf("%s\n", encoded_symbols);
	printf("%d\n", bits_in_last_byte);

	huffman_node_t *cur_node = root;
	uchar decoded_text[BUF_SIZE];
	decoded_text[0] = '\0';
	int significant_size = strlen(encoded_symbols) - (CHAR_SIZE_IN_BITS - bits_in_last_byte);

	for(int i = 0; i < significant_size; i++){
		if(encoded_symbols[i] == '1'){
				cur_node = cur_node->left_child; 
		}else{
				cur_node = cur_node->right_child;
		}

		if(is_leaf(cur_node)){
			int cur_size = strlen(&decoded_text[0]);
			decoded_text[cur_size] = cur_node->symbol;
			decoded_text[cur_size + 1] = '\0';
			cur_node = root;
			
		} 
	}

	printf("%s", decoded_text);

	
	free(encoded_text);
	clear_tree(root);
	fclose(in_file);
	fclose(out_file);
	// 10011001 01101011 11101010 11000100 11111100 11011000 10000000 000
	//	153			107    
}