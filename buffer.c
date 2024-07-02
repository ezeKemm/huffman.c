#include <string.h>
#include <stdio.h>

#include "buffer.h"

// Returns a FILE interface from a given path, fails otherwise
FILE* open_file(char* file, char* access) {
	FILE* file_in = fopen(file, access);
	if (file_in == NULL) {
		printf("Error: Failed to load file\n");
		exit(EXIT_FAILURE);
	}
	return file_in;
}

/* Reads data from file stream to input buffer 
 *
 * Receives file stream and ptr to empty buffer string
 * Fills buffer dynamically
 * Returns nothing							 */
size_t read_from_file(FILE* fp_in, char** bufr) {
	// size_t read_size = 0;
	/* Seek to end of file */
	if (fseek(fp_in, 0L, SEEK_END) == 0) {
		/* Get file size */
		long bufr_size = ftell(fp_in);
		// printf("%lu\n", bufr_size);
		if (bufr_size == -1) {
			/* Handle error */
			printf("Error: Getting file size failed\n");
			exit(EXIT_FAILURE);
		}

		/* allocate bufr of file size to memory */
		*bufr = malloc(sizeof(char) * (bufr_size + 1));

		/* Return cursor to file start */
		if (fseek(fp_in, 0L, SEEK_SET) != 0) {
			/* Handle error*/
			printf("Error: Moving file pointer to file start failed\n");
			exit(EXIT_FAILURE);
		}

		size_t read_size = fread(*bufr, sizeof(char), bufr_size, fp_in);
		// printf("Size: %lu\n", read_size);
		if (ferror(fp_in) != 0) {
			/* Handle error */
			fputs("Error: Reading file failed\n", stderr);
			exit(EXIT_FAILURE);
		// } else {
		// FIX: segfaulting with "test" case but not "testing!" or longer case ...
			//
			// *bufr[++read_size] = '\0'; /* Null terminate string for safety */
		}
		fclose(fp_in);		/* Close input file */
		return read_size;
	}
	return -1;
}

// WARN: UNTESTED
void read_from_string(char arr[], char** bufr) {
	// NOTE: is there any sideffect to using strlen since
	// it searches for a null-terminating character?
	// can it return the wrong size?
	int len = strlen(arr);	/* get string length */

	if (len <= 0) {
		/* Handle Error: invalid string */
		printf("Error: Cannot read stream of size 0");
		return;
	}
	/* allocate memory for buffer */
	*bufr = malloc(sizeof(char) * (len+1));

	/* copy string to buffer */
	for (int i = 0; i < len; i++) {
		*bufr[i] = arr[i];
	}
	*bufr[len] = '\0';		/* for safety */
}
