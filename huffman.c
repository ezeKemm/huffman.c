#include <regex.h>
#include <stdio.h>

#include "huffman.h"
#include "heap.h"

/* Utility function */
/* Counts valid entries (freq != 0) in frequency table */
int get_entries(Item items[], int size) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (items[i].freq != 0) {
			count++;
		}
	}
	return count;
}

/* Parses an input stream to generate a frequency table */
void get_freqs(char* stream, Item items[], int size) {
	char c;				// character buffer
	int strm_ptr = 0;	// pointer to stream
	while (strm_ptr < size) {
		c = stream[strm_ptr];
		if (!items[c].freq++) {
			items[c].symbol = c;
		}
		strm_ptr++;
	}
}

/* Construct a Huffman Tree using Min-Heap Priority Queue
 * and provided frequency table for input stream
 * param1: Item arr[] : Array of characters and their frequencies
 * from the input stream
 * param2: size_t size : Size of arr[]
 */
// NOTE: something about how the tree is built has been changed
// this should have no effect on the integrity of the tree
// so long as all codes are deterministic still
HNode* build_huffman_tree(Item arr[], size_t size) {
	HNode *left, *right, *top;

	/* Count valid entries in Items array */
	int count = get_entries(arr, size);
	if (!(count > 0)) {
		/* Handle Error */
		printf("Error: No valid entries to build queue\n");
		exit(EXIT_FAILURE);
	}

	/* Allocate memory for parameters of Heap constructor */
	char* items = malloc(sizeof(char) * (count));
	int* freqs = malloc(sizeof(int) * (count));

	/* Insert valid entries into arrays */
	int idx = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i].freq != 0) {
			items[idx] = arr[i].symbol;
			freqs[idx++] = arr[i].freq;
		}
	}

	if (idx != count) {
		/* Handle Error */
		printf("Error: Imbalance in input and output arrays\n");
	}

	/* Create a priority queue using min-heap */
	MinHeap* heap = generateHeap(items,freqs, count);

	/* Build the Huffman tree */
	while(!checkSizeOne(heap)) {
		left = pop(heap);
		right = pop(heap);
	// WARN: should we worry about this dollar sign? should be fine since if 
	// a dollar sign does it appear it appears at a leaf
		top = newNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		insert(heap, top);
	}
	// WARN: at this point we really should free memory 
	// the minheap struct and its corresponding array are now void
	// the freqtable struct and its arrays are also void
	
	HNode* Htree = pop(heap);

	free(heap->array);  // NOTE: do we need to free this separately?
	free(heap);
	free(items);
	free(freqs);

	return Htree;
}

void set_code(Item items[], int arr[], int size, int idx) {
	for (int i = 0; i < size; ++i) {
		items[idx].code[i] = arr[i];
	}
}

void huffman_codes(HNode* root, Item items[], int code[], int count) {
	// NOTE: cannot use increment operator (++) here, creates
	// double count len.... why? 
	// can't revert increment recursively on stack trace?
	if (root->left) {
		code[count] = 0;
		huffman_codes(root->left, items, code, count+1);
	}

	if (root->right) {
		code[count] = 1;
		huffman_codes(root->right, items, code, count+1);
	}

	if (isLeaf(root)) {
		set_code(items, code, count, (int)root->item);
		items[root->item].size = count;
	}
}
