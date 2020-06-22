#ifndef _BASELINE_H_
#define _BASELINE_H_



#include<stdio.h>


#define C 3
#define H 256
#define W 256
#define M 20
#define K 3
#define Tm 5
#define Tl 1

typedef char kernel_t;

void baseline(int input[C][H][W], int kernel[M][K][K][C], int output[M][H][W]);

#endif

