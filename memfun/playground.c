#include <stdlib.h>
#include <stdio.h>

extern int _i_memcpy_256_unaligned(char* di, char* si, int dx);
//; linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0-7

int main()
{
	int size = 256 + 128 + 64 + 32 + 16 + 9;
  char* s1 = (char*)malloc(sizeof(char) * size);
  char* s0 = (char*)malloc(sizeof (char) * (size + 5));
  char* s2 = s0 + 5; //not 16 aligned!
  for(int i =0; i < size; i++)
      s1[i] = 'a' + i%26;

  _i_memcpy_256_unaligned(s2, s1, size);

  free(s2);
  free(s1);

	return 0;
}
