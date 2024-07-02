#ifndef _BUFFERH_
#define _BUFFERH_
#include <stdio.h>
#include <stdlib.h>


/* Declarations */
FILE* open_file(char* file, char* access);
size_t read_from_file(FILE* fp_in, char** bufr);

#endif
