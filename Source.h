#pragma once
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <locale.h>
#define ROR(x, shift) (x >> shift | x << (( 8 * sizeof(x)) - shift))
typedef unsigned char uchar;
struct uchar2 {
	uchar x;
	uchar y;
};
typedef struct  {
	uchar symbol_0;
	uchar symbol_1;
	size_t freq;
}decrypt_info;