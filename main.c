#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
#include "huffman.h"
#include "encoder.h"
#include "buffer.h"

#define MAX_DEPTH 50
#define TABLE_SIZE 256
#define PEOF 126

// TODO: Error handling that isn't shutdown

// typedef struct {
// 	char symbol;
// 	int freq;
// 	int code[MAX_DEPTH];
// 	int size;
// } Item;
//
// typedef struct {
// 	int size;
// 	char symbols[ARR_LEN];
// 	int freq[ARR_LEN];
// } FreqTable;
//
// typedef struct {
// 	FreqTable* table;
// 	HNode* tree;
// 	int* codes[ARR_LEN];
// 	int sizes[ARR_LEN];
// 	int size;
// } Huffman;

// int arr_includes(char* c, char arr[], size_t n);
// void print_symbols(char arr[], int size);
// void print_freq(int arr[], int size, int comma);
// FreqTable* get_freqs(FILE* file);
// HNode* build_huffman_tree(FreqTable* table);

static void print_huff_codes(HNode* root, int arr[], int top);

void get_fp_out(FILE* fp_out, char* file_in, char* access) {
	/* create encoded file */
	char* extn = ".huffman";						// encoded file has extension .huffman
	size_t size = (strlen(file_in)+strlen(extn));	// sizeof(char) == 1 byte
	char* filename_out = (char*)malloc(size);		// NOTE: should we do it this way?
	strcat(filename_out, file_in);		
	strcat(filename_out, ".huffman");	// ex: "example.txt.huffman"
	fp_out = fopen(filename_out, access);
	free(filename_out);
}

int main() {

	Item items[256] = {{0}};
	printf("Starting program ... \n");
	FILE* in = open_file("test.txt", "r");
	char* buffer = NULL;
	int read = read_from_file(in, &buffer);
	// printf("\"%s\"\n", buffer);

	get_freqs(buffer, items, read);

	// int peof = 126;
	// items[peof].symbol = peof;		/* Assign pseudo-EOF to char in table */
	// items[peof].freq = 1;

	// printf("Printing Items...\n");
	// for (int i = 0; i < 256; i ++) {
	// 	printf("Item %u { ", i);
	// 	printf("Char: %c, ", items[i].symbol);
	// 	printf("Freq: %u, ", items[i].freq);
	// 	printf("Code size: %u, ", items[i].size);
	// 	printf("Code: [ ");
	// 	for (int j = 0; j < items[i].size; j++) {
	// 		printf("%u, ", items[i].code[j]);
	// 	}
	// 	printf(" ] ");
	// 	printf("},\n");
	// }
	// printf("Ending Printing of Items...\n\n");

	HNode* root = build_huffman_tree(items, TABLE_SIZE);
	int code[MAX_DEPTH];
	int count = 0;
	huffman_codes(root, items, code, count);
	// PrintCodes(root, code, count);

	// for (int i = 0; i < 256; i++) {
	// 	if (items[i].freq) {
	// 		printf(" %c | ", items[i].symbol);
	// 		for (int j = 0; j < items[i].size; j++) {
	// 			printf("%d" , items[i].code[j]);
	// 		}
	// 		printf("\n");
	// 	}
	// }

	// FILE* fp_out = open_file("test.txt.huffman", "w");
	FILE* fp_out;

	get_fp_out(fp_out, "test.txt", "w");
	encode(items, buffer, fp_out, read);

	fclose(fp_out);
	free(buffer);
	free(root);
	printf("Exiting program ...\n");
	return 0;
}

static void print_huff_codes(HNode* root, int arr[], int top) {
	if (root->left) {
		arr[top] = 0;
		print_huff_codes(root->left, arr, top +1);
	}
	
	if (root->right) {
		arr[top] = 1;
		print_huff_codes(root->right, arr, top +1);
	}

	if(isLeaf(root)) {
		printf(" %c | ", root->item);
		for (int i = 0; i < top; i++) {
			printf("%d", arr[i]);
		}
		printf("\n");
	}
}

// int main(int argc, char* argv[]) {
// 	FILE *fp_in, *fp_out;
// 	char filename[75] = {0};
//
// 	Item items[TABLE_SIZE] = {{0}};		// in stack, malloc into heap maybe?
// 	
// 	/***************************/
// 	/*  CLI Argument Parsing   */ 
// 	/***************************/
//
// 	int arg = argc;			// counter
// 	char** argp = argv;		// ptr to argv
//
// 	// compile regex expression
// 	regex_t regex;
// 	int error;
// 	// NOTE: i hate chatGPT but it was right 
// 	// about this one somehow...
// 	char* pattern = "^.*\\.txt$";		// regex pattern for valid text file path
// 	if (regcomp(&regex, pattern, 0)){
// 		printf("Error: Could not compile patter: %s\n", pattern);
// 		exit(EXIT_FAILURE);
// 	}
// 	
// 	// HACK: failsafe while we improperly parse arguments
// 	if (arg > 10) {
// 		printf("How many fucking arguments are you going to give????\n");
// 		exit(EXIT_FAILURE);
// 	}
//
// 	int file_found = 0;		// flag file argument
// 	/* Parse command line arguments to program */
// 	/* REQUIRED: path to file for compression  */
// 	while (arg > 1) {
// 		if (strlen(argp[1]) > 75) {		// learn to name things well
// 			printf("Error: Invalid argument: Exceedes character limit :: %s", argp[1]);
// 			exit(EXIT_FAILURE);
// 		}
//
// 		error = regexec(&regex, argp[1], 0, NULL, 0);	// match arg to regex
// 		if (!error && !file_found) {		// if a match and filepath not yet specified
// 			printf("Match! : %s\n", argp[1]);
// 			strcpy(filename, argp[1]);		// will only accept first filepath
// 			file_found = 1;					// NOTE: no error returned for multiple filepath arguments
// 		} else if (error == REG_NOMATCH) {
// 			// search for other arguments
// 			// FIX: currently parses infinite number of arguments?
// 			printf("No match\n");
// 			if (argp[1][0] != '-') {
// 				printf("The fuck is this?: %s\n", argp[1]);
// 			} else {
// 				printf("Nah fam wtf IS THIS?: %s\n", argp[1]);
// 			}
// 		} else {
// 			char ebufr[100];								// error message buffer
// 			regerror(error, &regex, ebufr, sizeof(ebufr));	// get regex error
// 			fprintf(stderr, "Regex failed: %s\n", ebufr);	// log regex error
// 			// do anything else...?
// 		}
// 		arg--;		// update counter to track current array posn
// 		argp++;		// increment pointer to point to next element
// 	}
//
// 	
// 	// If arg < 2 OR no argument was a valid filepath
// 	// prompt the user for a file path
// 	// NOTE: somewhat repetitive of above bc we need 
// 	// to get a valid file path two distinct ways 
// 	// PERF: terribly slow, variable speed ... probably bc of 
// 	// the load of data its handling incorrectly
// 	if (!file_found) {
// 		int count = 0;
// 		printf("Please enter the file to be compressed:\t");
// 		while (!file_found) {
// 			char bufr_in[75] = {0};				// buffer for user input
// 			scanf("%s", bufr_in);				// get file path from user
// 			error = regexec(&regex, bufr_in, 0, NULL, 0);	// match string to regex
//
// 			if (!error && !file_found) {		// if a match and filepath not yet specified
// 				printf("Match! : %s\n", bufr_in);
// 				strcpy(filename, bufr_in);		// copy filename to program
// 				file_found = 1;
// 				break;							// exit loop successfully
// 			} 
//
// 			// else case
// 			char ebufr[100];								// error message buffer
// 			regerror(error, &regex, ebufr, sizeof(ebufr));	// get regex error
// 			fprintf(stderr, "Invalid file path: %s\n", ebufr);
//
// 			// Fail program to prevent infinite loop
// 			if (count++ > 10) {
// 				printf("Error: Number of attempts exceeded ... Exiting program\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		printf("\n");	// TODO: ?
// 	}
//
// 	regfree(&regex);	// free memory allocated to regex
//
// 	/****************************/
// 	/*	  Huffman Algorithm 	*/
// 	/****************************/
//
// 	fp_in = open_file(filename, "r");	// incorporate into loop prompting 
// 										// user for file?
// 	get_freqs(fp_in, items);
// 	int code[MAX_DEPTH];
// 	HNode* root = build_huffman_tree(items, TABLE_SIZE);
// 	huffman_codes(root, items, code, 0);
// 	
// 	/* create encoded file */
// 	char* extn = ".huffman";						// encoded file has extension .huffman
// 	size_t size = (strlen(filename)+strlen(extn));	// sizeof(char) == 1 byte
// 	char* filename_out = (char*)malloc(size);		// NOTE: should we do it this way?
// 	strcat(filename_out, filename);		
// 	strcat(filename_out, ".huffman");	// ex: "example.txt.huffman"
// 	
// 	fp_out = open_file(filename_out, "w");
// 	encode(items, fp_in, fp_out);
//
// 	FIX: remove fclose of input
// 	fclose(fp_in);
// 	fclose(fp_out);
// 	free(filename_out);
// 	
// 	// free(items);
// 	free(root);
// 	exit(EXIT_SUCCESS);
// }







// int arr_includes(char* c, char arr[], size_t n) {
// 	for (int i = 0; i < n; ++i) {
// 		if (arr[i] == *c) {
// 			return i;
// 		}
// 	}
// 	return -1;
// }
//
// void print_symbols(char arr[], int size) {
// 	for (int i = 0; i < size; i++) {
// 		if (i < size-1) {
// 			printf("%c, ", arr[i]);
// 		} else {
// 			printf("%c\n", arr[i]);
// 		}
// 	}
// }
//
// void print_freq(int arr[], int size, int comma) {
// 	// printf("entering printer...");
// 	for (int i = 0; i < size; i++) {
// 		if (i < size-1) {
// 			printf("%d", arr[i]);
// 			if (comma) {
// 				printf(", ");
// 			}
// 		} else {
// 			printf("%d\n", arr[i]);
// 		}
// 	}
// 	// printf("exiting printer\n");
// }

// FreqTable* get_freqs(FILE* file) {
// 	FreqTable* table = malloc(sizeof(FreqTable));
// 	table->size = 0;
//
// 	char c;
// 	int idx;
//
// 	while ((c = fgetc(file)) != EOF) {
// 		if ((idx = arr_includes(&c, table->symbols, ARR_LEN)) != -1 ) {
// 			if (idx < ARR_LEN) {
// 				++table->freq[idx]; 
// 			} else {
// 				printf("Error: Idx out of bounds\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		} else {
// 			if (table->size < ARR_LEN) {
// 				table->symbols[table->size] = c;
// 				++table->freq[table->size++];
// 			} else {
// 				printf("Exceeded array bounds\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 	}
// 	// print_symbols(table->symbols, table->size);
// 	// print_freq(table->freq, table->size, 1);
//
// 	return table;
// }

// HNode* build_huffman_tree(FreqTable* table) {
// 	HNode *left, *right, *top;
// 	MinHeap* heap = generateHeap(table->symbols, table->freq, table->size);
//
// 	while(!checkSizeOne(heap)) {
// 		
// 		left = pop(heap);
// 		right = pop(heap);
//
// 		top = newNode('$', left->freq + right->freq);
//
// 		top->left = left;
// 		top->right = right;
//
// 		insert(heap, top);
// 	}
// 	// WARN: at this point we really should free memory 
// 	// the minheap struct and its corresponding array are now void
// 	// the freqtable struct and its arrays are also void
// 	return pop(heap);
// }
//
// void print_huffman_codes(HNode* root, int arr[], int curr) {
// 	// printf("Start print...");
// 	if (root->left) {
// 		arr[curr] = 0;
// 		print_huffman_codes(root->left, arr, curr+1);
// 	}
// 	if (root->right) {
// 		arr[curr] = 1;
// 		print_huffman_codes(root->right, arr, curr+1);
// 	}
// 	if (isLeaf(root)) {
// 		if (root->item == 10) {
// 			printf("  \\n | ");
// 		} else if (root->item == 0) {
// 			printf("  \\0 | ");
// 		} else {
// 			printf("  %c  | ", root->item);
// 		}
// 		print_freq(arr, curr, 0);
// 		return;
// 	}
// 	// printf("end print\n");
// }
//
// void print_codes(Huffman* huff) {
// 	for (int i = 0; i < huff->table->size; ++i) {
// 		int* code = huff->codes[i];
// 		int size = sizeof(huff->codes[i]);
// 		printf("%d", size);
//
// 		for (int j = 0; j < size; ++j) {
// 			printf("%d", code[j]);
// 		}
// 	}
// }
//
// void encode(Huffman* huff, HNode* root, int arr[], int curr) {
// 	if (root->left) {
// 		arr[curr] = 0;
// 		encode(huff, root->left, arr, curr+1);
// 	}
// 	if (root->right) {
// 		arr[curr] = 1;
// 		encode(huff, root->right, arr, curr+1);
// 	}
// 	if (isLeaf(root)) {
// 		int idx = arr_includes(&root->item, huff->table->symbols, huff->size);
// 		printf("%u", idx);
//
// 		if (idx != -1) { 
// 			huff->codes[idx] = arr;
// 			huff->sizes[idx] = curr;
// 			
// 			// print table entry
// 			printf("  %c  | ", root->item);
// 			for (int i = 0; i < curr; ++i) {
// 				printf("%u", arr[i]);
// 			}
// 			printf(" \n");
// 		}
// 		// print_freq(arr, curr, 0);
// 	}
// }
//
// Huffman* huffman(FreqTable* table) {
// 	Huffman* huff = malloc(sizeof(Huffman));
// 	huff->tree = build_huffman_tree(table);
// 	huff->size = table->size;
//
// 	int arr[MAX_DEPTH], curr = 0;
//
// 	// printf("starting job...\n");
// 	print_huffman_codes(huff->tree, arr, curr);
// 	// printf("end job\n");
// 	// int arr2[MAX_DEPTH], top = 0;
// 	// encode(huff, huff->tree, arr2, top);
//
// 	return huff;
// }
//
// int main() {
// 	FILE* file = open_file("test.txt");
// 	size_t size = ARR_LEN;
// 	Item* items[size];
// 	FreqTable* table = get_freqs(file);
// 	// printf("%d\n", table->size);
// 	Huffman* huff = huffman(table);
//
// 	
//
//
// 	free(file);
// 	free(huff);
// 	// free(items);
// 	exit(EXIT_SUCCESS);
//
// 	// get frequencies
// 	// build tree
// 	// build frequency table
// 	// -> traverse the tree, construct huffman code as C string
// 	// -> when leaf reached, insert huffman code into array
// }
