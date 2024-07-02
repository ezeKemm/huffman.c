#include "encoder.h"
#include <stdio.h>

void encode(Item items[], char* buf_in, FILE* fp_out, int size_in) {
	unsigned char cbuf = '\0';	/* character buffer */
	int i, out_size = 0;		/* stream counter, output size */ 
	int k = 0;					/* bit counter */

	/* calculate and store compressed file size */
	for (i = 0; i < 256; i++)	
		if (items[i].freq)		
			out_size += (items[i].freq * items[i].size);	

	// TODO: include frequency table for decoding
	
	// fprintf(fp_out, "%c", 'a');
	// fprintf(fp_out, "%04d\n", out_size);	// write file size to file  // TODO: keep?
	// fprintf(fp_out, "%c\n", (char)4);		// ASCII: Start-of-Text character
	
	// TODO: make output a buffer stream
	
	int size_actual = 0;
	/* encode input data into compressed file stream */
	for (i = 0; i < size_in; i++) {
		int* code = items[buf_in[i]].code;
		int n = items[buf_in[i]].size;
		int j = 0;				/* reset hcode counter */

		/* Scan through the huffman code for the 
		 * current char in the input stream		*/
		while (j < n) {
			/* Divide encoded input stream into 8 bit chunks and
			 * write hcode bit by bit into a character buffer.
			 * Write to cbuf by left shifting code bit to correct
			 * posn in byte and performing bitwise OR with buffer	*/
			cbuf = cbuf | (code[j++] << (7-k++));	// one-line update of counters
			if (k == 8) {
				/* Every 8 bits of huffman code is written to the output stream
				 * as the corrsponding 1 byte char		*/
				fprintf(fp_out, "%c", cbuf);	/* write to output stream */
				k = 0;			/* Reset bit counter */
				cbuf = '\0';	/* Reset char buffer */
			}
		}
	}
	/* partially filled cbuf at input stream end is already
	 * padded, simply write to output stream	*/
	fprintf(fp_out, "%c", cbuf);
	fprintf(fp_out, "%c", '\n');	/* terminate file */
	// PERF: any reason to use fputc instead here?
}
