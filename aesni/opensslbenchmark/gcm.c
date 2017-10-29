#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/opensslv.h>
#include <openssl/sha.h>
#include <string.h>

//#ifdef __linux__
#include <time.h>
//#endif
typedef unsigned char UCHAR;

typedef enum modes {
	AES_CTR,
	AES_CFB,
	AES_GCM
} MODES;

double timediff(long start_tv_sec, long start_tv_nsec, long end_tv_sec, long end_ntv_sec);

int compareArray(unsigned char res[], unsigned char exp[], int len) {
	for(int i=0; i< len; i++){
		if(res[i] != exp[i]) {
			printf("%s\n", "failed!");
			return 1;
		}
	}
	printf("%s\n", "passed");
	return 0;
}

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int gcm_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aad,
	int aad_len, unsigned char *key, int key_len_in_bytes, unsigned char *iv, int iv_len,
	unsigned char *ciphertext, unsigned char *tag)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;


	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the encryption operation. */
	if(key_len_in_bytes == 32) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 24) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_192_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 16) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
			handleErrors();
	}

	/* Set IV length if default 12 bytes (96 bits) is not appropriate */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	//just for testing
	//if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
    //	handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
		handleErrors();

	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

	/* Finalise the encryption. Normally ciphertext bytes may be written at
	 * this stage, but this does not occur in GCM mode
	 */
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;

	/* Get the tag */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
		handleErrors();

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *aad,
	int aad_len, unsigned char *tag, unsigned char *key, int key_len_in_bytes, unsigned char *iv, int iv_len,
	unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	int ret;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the decryption operation. */
	if(key_len_in_bytes == 32) {
		if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		handleErrors();
	}
	if(key_len_in_bytes == 24) {
		if(!EVP_DecryptInit_ex(ctx, EVP_aes_192_gcm(), NULL, NULL, NULL))
		handleErrors();
	}
	if(key_len_in_bytes == 16) {
		if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
		handleErrors();
	}


	/* Set IV length. Not necessary if this is 12 bytes (96 bits) */
	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(!EVP_DecryptUpdate(ctx, NULL, &len, aad, aad_len))
		handleErrors();

	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, 16))
		handleErrors();
	if(!EVP_DecryptUpdate(ctx, plaintext+16, &len, ciphertext+16, ciphertext_len-16))
		handleErrors();
	plaintext_len = len;

	/* Set expected tag value. Works in OpenSSL 1.0.1d and later */
	//if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
	//	handleErrors();

	/* Finalise the decryption. A positive return value indicates success,
	 * anything else is a failure - the plaintext is not trustworthy.
	 */
	ret = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	if(ret > 0)
	{
		/* Success */
		plaintext_len += len;
		return plaintext_len;
	}
	else
	{
		/* Verify failed */
		return -1;
	}
}

int test_gcm() {
	/* AES-GCM test data from NIST public test vectors */
	unsigned char gcm_key[] = {
		0xee, 0xbc, 0x1f, 0x57, 0x48, 0x7f, 0x51, 0x92, 0x1c, 0x04, 0x65, 0x66,
		0x5f, 0x8a, 0xe6, 0xd1, 0x65, 0x8b, 0xb2, 0x6d, 0xe6, 0xf8, 0xa0, 0x69,
		0xa3, 0x52, 0x02, 0x93, 0xa5, 0x72, 0x07, 0x8f
	};

	unsigned char gcm_iv[] = {
		0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
	};

	unsigned char gcm_pt[] = {
		0xf5, 0x6e, 0x87, 0x05, 0x5b, 0xc3, 0x2d, 0x0e, 0xeb, 0x31, 0xb2, 0xea,
		0xcc, 0x2b, 0xf2, 0xa5
	};

	unsigned char gcm_aad[] = {
		0x4d, 0x23, 0xc3, 0xce, 0xc3, 0x34, 0xb4, 0x9b, 0xdb, 0x37, 0x0c, 0x43,
		0x7f, 0xec, 0x78, 0xde
	};

	unsigned char gcm_ct[] = {
		0xf7, 0x26, 0x44, 0x13, 0xa8, 0x4c, 0x0e, 0x7c, 0xd5, 0x36, 0x86, 0x7e,
		0xb9, 0xf2, 0x17, 0x36
	};

	unsigned char gcm_tag[] = {
		0x67, 0xba, 0x05, 0x10, 0x26, 0x2a, 0xe4, 0x87, 0xd7, 0x37, 0xee, 0x62,
		0x98, 0xf7, 0x7e, 0x0c
	};

	unsigned char outbuf[1024]={0};
	unsigned char tag[32]={0};

	gcm_encrypt(gcm_pt, sizeof(gcm_pt), gcm_aad,
			sizeof(gcm_aad), gcm_key, sizeof(gcm_key), gcm_iv, 12,
			outbuf, tag);
}

int nist_gcm_case5() {

	//16 * 8
	unsigned char gcm_key[] = {
		0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08
	};

	//60 * 8
	unsigned char gcm_iv[] = { //cafebabefacedbad
		0x93,0x13,0x22,0x5d,0xf8,0x84,0x06,0xe5,0x55,0x90,0x9c,0x5a,0xff,0x52,0x69,0xaa,
	};

	unsigned char gcm_pt[] = {
		0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
        0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
        0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
        0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39
	};

	unsigned char gcm_aad[] = {
		0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef
        ,0xab,0xad,0xda,0xd2
	};

    /*"61353b4c2806934a777ff51fa22a4755"
                + "699b2a714fcdc6f83766e5f97b6c7423"
                + "73806900e49f24b22b097544d4896b42"
                + "4989b5e1ebac0f07c23f4598";
    */
	unsigned char gcm_ct[] = {
		0x8c,0xe2,0x49,0x98,0x62,0x56,0x15,0xb6,0x03,0xa0,0x33,0xac,0xa1,0x3f,0xb8,0x94,
        0xbe,0x91,0x12,0xa5,0xc3,0xa2,0x11,0xa8,0xba,0x26,0x2a,0x3c,0xca,0x7e,0x2c,0xa7,
        0x01,0xe4,0xa9,0xa4,0xfb,0xa4,0x3c,0x90,0xcc,0xdc,0xb2,0x81,0xd4,0x8c,0x7c,0x6f,
        0xd6,0x28,0x75,0xd2,0xac,0xa4,0x17,0x03,0x4c,0x34,0xae,0xe5
	};

    // "3612d2e79e3b0785561be14aaca2fccb"
	unsigned char gcm_tag[] = {
		0x61,0x9c,0xc5,0xae,0xff,0xfe,0x0b,0xfa,0x46,0x2a,0xf4,0x3c,0x16,0x99,0xd0,0x50
	};

	unsigned char outbuf[1024];
	unsigned char tag[32];

	gcm_encrypt(gcm_pt, sizeof(gcm_pt), gcm_aad,
			sizeof(gcm_aad), gcm_key, sizeof(gcm_key), gcm_iv, 60,
			outbuf, tag);

	compareArray(outbuf, gcm_ct, 60);
	compareArray(tag, gcm_tag, 16);
}



int nist_gcm_case6() {

	//16 * 8
	unsigned char gcm_key[] = {
		0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08
	};

	//60 * 8
	unsigned char gcm_iv[] = {
		0x93,0x13,0x22,0x5d,0xf8,0x84,0x06,0xe5,0x55,0x90,0x9c,0x5a,0xff,0x52,0x69,0xaa,
        0x6a,0x7a,0x95,0x38,0x53,0x4f,0x7d,0xa1,0xe4,0xc3,0x03,0xd2,0xa3,0x18,0xa7,0x28,
        0xc3,0xc0,0xc9,0x51,0x56,0x80,0x95,0x39,0xfc,0xf0,0xe2,0x42,0x9a,0x6b,0x52,0x54,
        0x16,0xae,0xdb,0xf5,0xa0,0xde,0x6a,0x57,0xa6,0x37,0xb3,0x9b
	};

	unsigned char gcm_pt[] = {
		0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
        0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
        0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
        0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39
	};

	unsigned char gcm_aad[] = {
		0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef
        ,0xab,0xad,0xda,0xd2
	};

	unsigned char gcm_ct[] = {
		0x8c,0xe2,0x49,0x98,0x62,0x56,0x15,0xb6,0x03,0xa0,0x33,0xac,0xa1,0x3f,0xb8,0x94,
        0xbe,0x91,0x12,0xa5,0xc3,0xa2,0x11,0xa8,0xba,0x26,0x2a,0x3c,0xca,0x7e,0x2c,0xa7,
        0x01,0xe4,0xa9,0xa4,0xfb,0xa4,0x3c,0x90,0xcc,0xdc,0xb2,0x81,0xd4,0x8c,0x7c,0x6f,
        0xd6,0x28,0x75,0xd2,0xac,0xa4,0x17,0x03,0x4c,0x34,0xae,0xe5
	};

	unsigned char gcm_tag[] = {
		0x61,0x9c,0xc5,0xae,0xff,0xfe,0x0b,0xfa,0x46,0x2a,0xf4,0x3c,0x16,0x99,0xd0,0x50
	};

	unsigned char outbuf[1024];
	unsigned char tag[32];

	gcm_encrypt(gcm_pt, sizeof(gcm_pt), gcm_aad,
			sizeof(gcm_aad), gcm_key, sizeof(gcm_key), gcm_iv, 60,
			outbuf, tag);

	compareArray(outbuf, gcm_ct, 60);
	compareArray(tag, gcm_tag, 16);
}

int nist_gcm_case6_decrypt() {
	//16 * 8
	unsigned char gcm_key[] = {
		0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08
	};

	//60
	unsigned char gcm_iv[] = {
		0x93,0x13,0x22,0x5d,0xf8,0x84,0x06,0xe5,0x55,0x90,0x9c,0x5a,0xff,0x52,0x69,0xaa,
        0x6a,0x7a,0x95,0x38,0x53,0x4f,0x7d,0xa1,0xe4,0xc3,0x03,0xd2,0xa3,0x18,0xa7,0x28,
        0xc3,0xc0,0xc9,0x51,0x56,0x80,0x95,0x39,0xfc,0xf0,0xe2,0x42,0x9a,0x6b,0x52,0x54,
        0x16,0xae,0xdb,0xf5,0xa0,0xde,0x6a,0x57,0xa6,0x37,0xb3,0x9b
	};

	unsigned char gcm_pt[] = {
		0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
        0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
        0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
        0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39
	};

	unsigned char gcm_aad[] = {
		0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef
        ,0xab,0xad,0xda,0xd2
	};

	unsigned char gcm_ct[] = {
		0x8c,0xe2,0x49,0x98,0x62,0x56,0x15,0xb6,0x03,0xa0,0x33,0xac,0xa1,0x3f,0xb8,0x94,
        0xbe,0x91,0x12,0xa5,0xc3,0xa2,0x11,0xa8,0xba,0x26,0x2a,0x3c,0xca,0x7e,0x2c,0xa7,
        0x01,0xe4,0xa9,0xa4,0xfb,0xa4,0x3c,0x90,0xcc,0xdc,0xb2,0x81,0xd4,0x8c,0x7c,0x6f,
        0xd6,0x28,0x75,0xd2,0xac,0xa4,0x17,0x03,0x4c,0x34,0xae,0xe5
	};

	unsigned char gcm_tag[] = {
		0x61,0x9c,0xc5,0xae,0xff,0xfe,0x0b,0xfa,0x46,0x2a,0xf4,0x3c,0x16,0x99,0xd0,0x50
	};

	unsigned char outbuf[1024] ={0};
	unsigned char tag[32]={0};

	//tamper ...
	gcm_ct[0] = gcm_ct[0] + 1;

	int ret = gcm_decrypt(gcm_ct, sizeof(gcm_ct), gcm_aad, sizeof(gcm_aad), gcm_tag, gcm_key, sizeof(gcm_key), gcm_iv, sizeof(gcm_iv), outbuf);
	if(ret < 0) {
        printf("verify failed!\n");
    }
	compareArray(outbuf, gcm_pt, sizeof(gcm_pt));
	//compareArray(tag, gcm_tag, 16);
}



void printResult(const char*mode, int total_size, int chunk_size, int key_len_in_bytes, double interval_sec) {


	printf("OpenSSL %s Encryption(Total=%dMB, key=%dbits, Chunk=%3d%s)\tthroughput= %.2fMB/s.\n",
			mode,
			total_size/(1024*1024),
			key_len_in_bytes * 8,
			chunk_size>=1024*1024 ? chunk_size/(1024*1024) : chunk_size/1024,
			chunk_size>=1024*1024 ? "MB" : "KB",
			(double)total_size/(1024*1024*interval_sec) );
}

/* return sonsumed time in seconds*/
double ctr_encrypt_benchmark(int total_size, int chunk_size, int key_len_in_bytes, MODES mode) {

	int chunk_num = total_size/chunk_size;

	UCHAR* key = (UCHAR*) malloc(sizeof(UCHAR) * key_len_in_bytes);
	UCHAR* iv = (UCHAR*) malloc(sizeof(UCHAR) * 16);
	UCHAR* aad = (UCHAR*) malloc(sizeof(UCHAR) * 16);
	int aad_len = 16;

	UCHAR** plaintext_arr = (UCHAR**)malloc(sizeof(UCHAR*) * chunk_num);
	for(int i=0; i < chunk_num; i++) {
		plaintext_arr[i] = (UCHAR*) malloc(sizeof(UCHAR) * chunk_size);
	}

	UCHAR* ciphertext = (UCHAR*) malloc(sizeof(UCHAR) * (total_size + 16));
	UCHAR* tag = (UCHAR*) malloc(sizeof(UCHAR) * 16);
	if(key == NULL || iv == NULL || aad == NULL || plaintext_arr == NULL || ciphertext == NULL || tag == NULL) {
		return 1;
	}

	//random initialization
	time_t t;
 	srand((unsigned) time(&t));
	for (int i = 0; i < key_len_in_bytes; ++i){
		key[i]=(UCHAR) (rand() % 256);
	}
	for (int i = 0; i < 16; ++i) {
		iv[i]=(UCHAR) (rand() % 256);
	}
	for (int i = 0; i < 16; ++i) {
		aad[i]=(UCHAR) (rand() % 256);
	}
	for(int i=0; i < chunk_num; i++) {
		for(int j=0; j<chunk_size; j++)
			plaintext_arr[i][j] = (UCHAR) (rand() % 256);
	}

	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the encryption operation. */
	if(mode == AES_CTR) {
		if(key_len_in_bytes == 32) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, NULL, NULL))
				handleErrors();
		}
		if(key_len_in_bytes == 24) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_192_ctr(), NULL, NULL, NULL))
				handleErrors();
		}
		if(key_len_in_bytes == 16) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, NULL, NULL))
				handleErrors();
		}
	} else if(mode == AES_CFB) {
		if(key_len_in_bytes == 32) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, NULL, NULL))
				handleErrors();
		}
		if(key_len_in_bytes == 24) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_192_cfb(), NULL, NULL, NULL))
				handleErrors();
		}
		if(key_len_in_bytes == 16) {
			if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, NULL, NULL))
				handleErrors();
		}
	}

	/* Set IV length if default 12 bytes (96 bits) is not appropriate */
	//if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
	//	handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	//if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
	//	handleErrors();

	//Only accumulate encryption & ghash time

	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	// Encryption
	for (int i = 0; i < chunk_num ; ++i) {
		if(1 != EVP_EncryptUpdate(ctx, ciphertext + i*chunk_size, &len, plaintext_arr[i], chunk_size)) handleErrors();
		ciphertext_len += len;
	}
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + total_size, &len)) handleErrors();
	ciphertext_len += len;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

	double interval_sec  = timediff(start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);

	/* Get the tag */
	//if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
	//	handleErrors();

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	/* clean ------*/
	free(key);
	free(iv);
	free(aad);
	free(ciphertext);
	free(tag);
	for(int i=0; i< chunk_num; i++) {
		free(plaintext_arr[i]);
	}
	free(plaintext_arr);

	return interval_sec;
}



int ctr_throughput(MODES mode){

	double interval_sec = 0.0;
	int total_size = 1024*1024*1024;
	int chunk_size_arr[3] = {1024*16,	1024*1024 /*,	1024*1024*32*/};
	int key_len_arr[3] = {16, 24, 32};
	int itr_num = 3;

	for(int k=0; k<3; k++) { // key length
		for(int i=0; i<3; i++) { // chunk size
			interval_sec = 0.0;
			for(int itr = 0; itr<itr_num; itr++) {
				interval_sec += ctr_encrypt_benchmark(total_size, chunk_size_arr[i], key_len_arr[k], mode);
			}
			printResult("CTR", total_size, chunk_size_arr[i], key_len_arr[k], interval_sec/itr_num);
		}
	}

	return 0;
}

/* return sonsumed time in seconds*/
double gcm_encrypt_benchmark(int total_size, int chunk_size, int key_len_in_bytes) {

	int chunk_num = total_size/chunk_size;

	UCHAR* key = (UCHAR*) malloc(sizeof(UCHAR) * key_len_in_bytes);
	UCHAR* iv = (UCHAR*) malloc(sizeof(UCHAR) * 12);
	UCHAR* aad = (UCHAR*) malloc(sizeof(UCHAR) * 16);
	int aad_len = 16;

	UCHAR** plaintext_arr = (UCHAR**)malloc(sizeof(UCHAR*) * chunk_num);
	for(int i=0; i < chunk_num; i++) {
		plaintext_arr[i] = (UCHAR*) malloc(sizeof(UCHAR) * chunk_size);
	}

	UCHAR* ciphertext = (UCHAR*) malloc(sizeof(UCHAR) * (total_size + 16));
	UCHAR* tag = (UCHAR*) malloc(sizeof(UCHAR) * 16);
	if(key == NULL || iv == NULL || aad == NULL || plaintext_arr == NULL || ciphertext == NULL || tag == NULL) {
		return 1;
	}

	//random initialization
	time_t t;
 	srand((unsigned) time(&t));
	for (int i = 0; i < key_len_in_bytes; ++i){
		key[i]=(UCHAR) (rand() % 256);
	}
	for (int i = 0; i < 12; ++i) {
		iv[i]=(UCHAR) (rand() % 256);
	}
	for (int i = 0; i < 16; ++i) {
		aad[i]=(UCHAR) (rand() % 256);
	}
	for(int i=0; i < chunk_num; i++) {
		for(int j=0; j<chunk_size; j++)
			plaintext_arr[i][j] = (UCHAR) (rand() % 256);
	}

	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the encryption operation. */
	if(key_len_in_bytes == 32) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 24) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_192_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 16) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
			handleErrors();
	}

	/* Set IV length if default 12 bytes (96 bits) is not appropriate */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
		handleErrors();

	//Only accumulate encryption & ghash time

	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	// Encryption
	for (int i = 0; i < chunk_num ; ++i) {
		if(1 != EVP_EncryptUpdate(ctx, ciphertext + i*chunk_size, &len, plaintext_arr[i], chunk_size)) handleErrors();
		ciphertext_len += len;
	}
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + total_size, &len)) handleErrors();
	ciphertext_len += len;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

	double interval_sec  = timediff(start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);

	/* Get the tag */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
		handleErrors();

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	/* clean ------*/
	free(key);
	free(iv);
	free(aad);
	free(ciphertext);
	free(tag);
	for(int i=0; i< chunk_num; i++) {
		free(plaintext_arr[i]);
	}
	free(plaintext_arr);

	return interval_sec;
}

double GMAC(UCHAR* data, int data_len, int key_len_in_bytes){
	//nt key_len_in_bytes = 16;

	UCHAR* key = (UCHAR*) malloc(sizeof(UCHAR) * key_len_in_bytes);
	UCHAR* iv = (UCHAR*) malloc(sizeof(UCHAR) * 12);

	UCHAR* tag = (UCHAR*) malloc(sizeof(UCHAR) * 16);

	if(key == NULL || iv == NULL || data == NULL || tag == NULL) {
		return 0;
	}

	//random initialization
	time_t t;
 	srand((unsigned) time(&t));
	for (int i = 0; i < key_len_in_bytes; ++i){
		key[i]=(UCHAR) (rand() % 256);
	}
	for (int i = 0; i < 12; ++i) {
		iv[i]=(UCHAR) (rand() % 256);
	}

	EVP_CIPHER_CTX *ctx;
	int len;

	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the encryption operation. */
	if(key_len_in_bytes == 32) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 24) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_192_gcm(), NULL, NULL, NULL))
			handleErrors();
	}
	if(key_len_in_bytes == 16) {
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL))
			handleErrors();
	}

	/* Set IV length if default 12 bytes (96 bits) is not appropriate */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide any AAD data. This can be called zero or more times as
	 * required
	 */
	if(1 != EVP_EncryptUpdate(ctx, NULL, &len, data, data_len))
		handleErrors();

	if(1 != EVP_EncryptFinal_ex(ctx, NULL, &len)) handleErrors();

	/* Get the tag */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
		handleErrors();

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	double interval_sec  = timediff(start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);

	/* clean ------*/
	free(key);
	free(iv);
	free(tag);

	return interval_sec;
}
//-----

double GMAC_benchmark(int chunk_size, int key_len_in_bytes) {

	time_t t;
 	srand((unsigned) time(&t));

 	double interval_sec = 0.0;
 	//int chunk_size = 1024*64; //64K
 	int itr_num = 1024;

	for(int i = 0; i< itr_num; i++) {
		int data_len = chunk_size;
		UCHAR* data = (UCHAR*) malloc(sizeof(UCHAR) * data_len);
		//random initialization
		for (int i = 0; i < data_len; ++i){
			data[i]=(UCHAR) (rand() % 256);
		}

		interval_sec += GMAC(data, data_len, key_len_in_bytes);

		free (data);
	}
	printResult("GMAC", chunk_size * itr_num, chunk_size, key_len_in_bytes, interval_sec);
}


//sha256
double sha256(UCHAR* data, int data_len)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

		struct timespec start_time, end_time;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, data_len);
    SHA256_Final(hash, &sha256);

		//EVP_CIPHER_CTX_free(ctx);

		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
		double interval_sec  = timediff(start_time.tv_sec, start_time.tv_nsec, end_time.tv_sec, end_time.tv_nsec);

    // int i = 0;
    // for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    // {
    //     //sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    // }
    return interval_sec;
}
double sha256_benchmark(int chunk_size) {

	time_t t;
 	srand((unsigned) time(&t));

 	double interval_sec = 0.0;
 	//int chunk_size = 1024*64; //64K
 	int itr_num = 1024;

	for(int i = 0; i< itr_num; i++) {
		int data_len = chunk_size;
		UCHAR* data = (UCHAR*) malloc(sizeof(UCHAR) * data_len);
		//random initialization
		for (int i = 0; i < data_len; ++i){
			data[i]=(UCHAR) (rand() % 256);
		}

		interval_sec += sha256(data, data_len);

		free (data);
	}
	printResult("SHA256", chunk_size * itr_num, chunk_size, 32, interval_sec);
}
//======================

int gcm_throughput(){

	double interval_sec = 0.0;
	int total_size = 1024*1024*1024;
	int chunk_size_arr[3] = {1024*16,	1024*1024,	1024*1024*32};
	int key_len_arr[3] = {16, 24, 32};
	int itr_num = 3;

	for(int k=0; k<3; k++) { // key length
		for(int i=0; i<3; i++) { // chunk size
			interval_sec = 0.0;
			for(int itr = 0; itr<itr_num; itr++) {
				interval_sec += gcm_encrypt_benchmark(total_size, chunk_size_arr[i], key_len_arr[k]);
			}
			printResult("GCM", total_size, chunk_size_arr[i], key_len_arr[k], interval_sec/itr_num);
		}
	}

	return 0;
}

void warm_up() {
	gcm_encrypt_benchmark(1024*1024*1024, 1024, 16);
	gcm_encrypt_benchmark(1024*1024*1024, 1024, 24);
	gcm_encrypt_benchmark(1024*1024*1024, 1024, 32);
}

int main (void)
{
	printf("%s\n",SSLeay_version(0));
	printf("%s\n",SSLeay_version(1));
	printf("%s\n",SSLeay_version(2));
	printf("%s\n",SSLeay_version(3));
	printf("%s\n",OPENSSL_VERSION_TEXT);
	printf("0x%x\n",OPENSSL_VERSION_NUMBER);

	//test_gcm();
	nist_gcm_case6();
	//warm_up();

	GMAC_benchmark(1024*64, 32);
	GMAC_benchmark(1024*1024, 32);

	sha256_benchmark(1024*64);
	sha256_benchmark(1024*1024);

	gcm_throughput();
	ctr_throughput(AES_CTR);
	ctr_throughput(AES_CFB);

	//nist_gcm_case6_decrypt();

	return 0;
}

/* return seconds */
double timediff(long start_tv_sec, long start_tv_nsec, long end_tv_sec, long end_ntv_sec)
{
	long tmp_nsec = 0;
	long tmp_sec = 0;
    if ((end_ntv_sec - start_tv_nsec) < 0)
    {
        tmp_sec = end_tv_sec - start_tv_sec - 1;
        tmp_nsec = 1000000000 + end_ntv_sec - start_tv_nsec;
    }
    else
    {
        tmp_sec = end_tv_sec - start_tv_sec;
        tmp_nsec = end_ntv_sec - start_tv_nsec;
    }
    return tmp_sec + ((double)tmp_nsec) / 1000000000;
}
