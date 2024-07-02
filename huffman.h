#ifndef _HUFFMANH_
#define _HUFFMANH_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

/* Size of code array as recursive depth of Huffman Tree */
#define MAX_DEPTH 50
/* The Standard + Extended ASCII table contains 256 entries */
/* C compilers can differ in their character code 
 * (i.e. mapping of character type to integer) */
#define TABLE_SIZE 256

typedef struct {
	char symbol;
	int freq;
	int size;
	int code[MAX_DEPTH];
} Item;

/* Utility Functions */
int get_entries(Item items[], int size);
void set_code(Item items[], int arr[], int size, int idx);
void print_huffman_codes(HNode* root, int arr[], int curr);

/* Generate Frequency table from a given input stream */
void get_freqs(char* stream, Item arr[], int size);

/* Construct Huffman Tree and Huffman Codes*/
HNode* build_huffman_tree(Item arr[], size_t n);
void huffman_codes(HNode* root, Item items[], int code[], int count);

#endif
