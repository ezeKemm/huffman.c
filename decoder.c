#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>

/* accidentally implemented atoi() lol */
int arr_to_int(char freq[], int size) {
	int res = 0;
	for (int i = 0; i < size; i++){
		res = (res * 10) + (freq[i] - 48);
	}
	return res;
}

int decode_header(char* buf_in) {
	
	Item items[256] = {{0}};
	int flag = 1;
	int bidx = 0;
	int c;
	char freq[16];
	while(flag) {
		c = buf_in[bidx];
		int fidx = 0;
		bidx++;
		while (buf_in[bidx] != ' ') {
			freq[fidx] = buf_in[bidx];  
			bidx++;
			fidx++;
		}
		items[c].freq =  atoi(freq);
	}
	return -1;
}

void decode(char* buf_in, FILE* fp_out) {
	/* scan thru buffer
	 * PARSE HEADER
	 * specification: 
	 * %c%u[:%u](space delimited)
	 * %c: character 
	 * %u: character frequency
	 * [:%u]: eof symbol flag + occurrence count
	 * build tree
	 * 
	 * DECODE:
	 *  for each character:
	 *	-> convert to binary
	 *	-> traverse thru tree with given binary
	 *	-> when reach leaf -> return character at leaf
	 *
	 * if eof character detected:
	 *	-> check if occurrence count is 0
	 *		-> EOF reached
	 *		-> end decoding
	 *	-> decode as normal character 
	 *	-> decrement occurrence count
	 *	
	*/

}
