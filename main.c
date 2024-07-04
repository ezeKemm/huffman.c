#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
#include "huffman.h"
#include "encoder.h"
#include "buffer.h"

#define MAX_DEPTH 50
#define TABLE_SIZE 256
#define PEOF 127

// TODO: Error handling that isn't shutdown

static void print_huff_codes(HNode* root, int arr[], int top);
void get_fp_out(FILE* fp_out, char* file_in, char* access);

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

int main() {
	printf("Starting program ... \n");
	FILE *fp_in, *fp_out;
	Item items[TABLE_SIZE] = {{0}};
	char* buffer = NULL;
	fp_in = open_file("test.txt", "r");
	int read = read_from_file(fp_in, &buffer);
	// buffer[read++] = PEOF;		/* insert pseudo-EOF into huffman tree */
	// HACK: track frequency of PEOF char in file ahead of time
	// n-1 occurrence is encoded and nth occurrence is EOF

	int code[MAX_DEPTH];
	int count = 0;
	int ceof = 0;
	HNode* root;
	get_freqs(buffer, items, read);

	// WARN: bc i can't be bothered to figure out how/why non-printable chars past 127 
	// don't actually to my knowledge work and are implicitly cast to negative 
	// integers; despite the architecture shouldn't do this? thank you gcc
	if (items[PEOF].freq) {
		printf("PEOF is a valid character\n");
		/* If PEOF already in file as valid character,
		 * store the frequency and build huffman
		 * code using pre-appended frequency */
		ceof = items[PEOF].freq;
		root = build_huffman_tree(items, TABLE_SIZE);
		huffman_codes(root, items, code, count);
		items[PEOF].freq++;
	} else {
		printf("PEOF is not a valid character...adding to tree\n");
		/* else, insert PEOF into table to generate code */
		items[PEOF].freq++;
		items[PEOF].symbol = PEOF;
		items[PEOF].peof++;		/* update PEOF flag */
		root = build_huffman_tree(items, TABLE_SIZE);
		huffman_codes(root, items, code, count);
	}
	items[PEOF].peof++;		/* set PEOF flag */
	buffer[read++] = PEOF;		/* append stream w/ pseudo-EOF */

	int printable[MAX_DEPTH];
	print_huff_codes(root, printable, 0);

	fp_out = open_file("test.txt.huffman", "w");
	// get_fp_out(fp_out, "test.txt", "w");
	encode(items, buffer, fp_out, read);

	fclose(fp_out);
	free(buffer);
	free(root);
	printf("Exiting program ...\n");
	return 0;
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
