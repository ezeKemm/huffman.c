#ifndef _HEAPH_
#define _HEAPH_

// NOTE: Use typedef to alias struct for 
// convenience in later reference

// TODO: DOCUMENT

// Node of the Min Heap contains the assigned character, 
// the corresponding frequency in the text stream, 
// and pointers to any child nodes (binary)
typedef struct HNode {
	char item;
	unsigned freq;
	struct HNode *left, *right;
} HNode;

// An implementation of the Min Heap Data Structure
typedef struct {
	unsigned size;
	unsigned capacity;
	struct HNode **array;
} MinHeap;

HNode* newNode(char item, unsigned freq);
MinHeap* newMinHeap(unsigned capacity);
void minHeapify(MinHeap  *heap, int idx);

HNode* pop(MinHeap* heap);
void insert(MinHeap* heap, HNode* node);

void swapNodes(HNode **a, HNode **b);

int checkSizeOne(MinHeap* heap);
int isLeaf(HNode* root);

MinHeap* generateHeap(char item[], int freq[], int size);


// TESTS
// a negative (signed) value in the heap?
// does it heapify correctly?
// behavior for equivalent values?
// memory management?

#endif
