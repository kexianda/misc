#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <time.h>
#endif //__linux__

#define BLOCK_SIZE 16

typedef unsigned char UCHAR;


//RDI, RSI, RDX, RCX, R8, R9, XMM0~7
extern test_mult(const UCHAR* op1, const UCHAR* op2, UCHAR* out);
extern test_mult_karatsuba(const UCHAR*op1, const UCHAR*op2, UCHAR*out);

int main() {
	int i = 0;
	UCHAR op1[BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
							 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	UCHAR op2[BLOCK_SIZE] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 
							 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
	UCHAR out[32] = {};
	UCHAR out2[32] = {};
	test_mult(op1, op2, out);
	test_mult_karatsuba(op1, op2, out2);

	for(i=0; i<32; i++) {
		printf("%0x,", out[i]);
	}
	printf("\n");
	for(i=0;i<32; i++){
		printf("%0x,",out2[i]);
	}
	printf("\n");
}
