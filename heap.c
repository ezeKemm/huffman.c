#include "heap.h"
#include <stdlib.h>

HNode* newNode(char item, unsigned freq) {
	// Allocate space to store Heap Node in memory
	// Coerce into a pointer to node in memory
	HNode* temp = (HNode*)malloc(sizeof(HNode));
	temp->left = temp->right = NULL;
	temp->item = item;
	temp->freq = freq;

	return temp; // return ptr
}

MinHeap* createMinHeap(unsigned capacity) {
	// Allocate space to store heap struct in memory
	MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	
	// Allocate space to store an array of "capacity" pointers in memory, 
	// where each element points to a heap node in memory.
	// This serves as our heap data structure, each node is stored in the array
	// TODO: memory management -> free this memory after
	// |-> new "method"?
	minHeap->array = (HNode**)malloc(minHeap->capacity * sizeof(HNode*));

	return minHeap;
}

// Swap pointer references of two nodes such that: 
// pointer A now points to Node B, 
// and pointer B now points to Node A.
void swapNodes(HNode **a, HNode **b) {
	HNode* temp = *a;
	*a = *b;
	*b = temp;
}

void minHeapify(MinHeap *heap, int idx) {
	int min = idx;
	int lchild = 2*idx + 1;
	int rchild = 2*idx + 2;

	// safety check: within array bounds
	if (lchild < heap->size && heap->array[lchild]->freq < heap->array[min]->freq) {
		// if left child is smaller than parent
		// rearrange so left child is new parent
		min = lchild;
	}
	
	// safety check: within array bounds
	if (rchild < heap->size && heap->array[rchild]->freq < heap->array[min]->freq) {
		// if right child is smaller than parent
		// rearrange so right child is new parent
		min = rchild;
	}

	// NOTE: Left and right children are unordered, such that it is not given that 
	// one child is greater than the other if the other child is less than the parent;
	// Min-heap rule requires only parent be less than both children.
	// When we set the new parent, we perform this check again recursively?
	
	// If a child is found to be < the parent (i.e. indices were swapped)
	if (min != idx) {
		// Swap nodes such that smallest node is at the top of the heap (idx 0)
		// NOTE: is this actually swapping array ptrs?
		swapNodes(&heap->array[min], &heap->array[idx]);
		minHeapify(heap, min); // perform heapify again with new minimum
	}
}

HNode* pop(MinHeap *heap) {
	HNode* temp = heap->array[0];
	heap->array[0] = heap->array[heap->size - 1];

	--heap->size;
	minHeapify(heap, 0);

	return temp;
}

void insert(MinHeap *heap, HNode *node) {
	++heap->size; // increment heap size by 1 for insertion;
	int i = heap->size - 1; // the last array element (lowest leaf node in tree)
	
	// start at the bottom of the tree and work up (greatest to least)
	// safety check: index is within bounds (unsigned whole int)
	// if node is less than parent node: move up the tree
	// in other words, while min-heap rule unsatisfied, "heapify"
	while (i && node->freq < heap->array[(i-1)/2]->freq) {
		// rearrange array elements, parent node moved down the tree
		heap->array[i] = heap->array[(i-1)/2];
		i = (i - 1) / 2;
	}

	heap->array[i] = node;
}

int checkSizeOne(MinHeap *heap) {
	return (heap->size == 1);
}

int isLeaf(HNode *root) {
	return !(root->left) && !(root->right);
}

MinHeap* generateHeap(char *item, int *freq, int size) {
	// create new heap instance
	MinHeap* heap = createMinHeap(size);
	
	// insert nodes into heap array
	// FIX: changes to freq and items array length creating segfault

	for(int i = 0; i < size; ++i) {
		heap->array[i] = newNode(item[i], freq[i]);
	}


	heap->size = size;

	// build the heap (i.e. heapify)
	// starting from bottom-up
	int n = heap->size - 1;
	int j;

	for (j = (n-1) / 2; j >= 0; --j) {
		minHeapify(heap, j);
	}

	// return newly generated heap
	return heap;
}

