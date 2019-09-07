#pragma once
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <locale.h>
#include <amp.h>
#define ROR(x, shift) (x >> shift | x << (( 8 * sizeof(x)) - shift))
typedef unsigned char uchar;
typedef unsigned short ushort;
struct uchar2 {
	uchar x;
	uchar y;
}; 
typedef struct  {
	union {
		char byte[32];
		double double_data[4];
		unsigned long long ulonglong_data[4];
		int integer[8];
	};
}bit_256;
typedef struct  {
	uchar symbol_0;
	uchar symbol_1;
	size_t freq;
}decrypt_info;