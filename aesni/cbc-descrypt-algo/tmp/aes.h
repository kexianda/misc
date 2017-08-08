#ifndef _AES_H
#define _AES_H

typedef unsigned char UCHAR;
  
#if (__cplusplus)
extern "C" {
#endif  


// linux ABI: RDI, RSI, RDX, RCX, R8, R9, XMM0-7
//expandedKey
int aes_cbc_decrypt(UCHAR *plainText,  UCHAR *cipherText,  UCHAR *key,  size_t length,  UCHAR *iv);


#if (__cplusplus)
} /*extern "C" */
#endif 
#endif  /*_AES_H*/
