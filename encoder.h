#ifndef _ENCODERH_
#define _ENCODERH_

#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

void encode(Item items[], char* buf_in, FILE* fp_out, int size_in);

 #endif
