#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

//#ifdef __linux__
#include <time.h>
//#endif
typedef unsigned char UCHAR;

double timediff(long start_tv_sec, long start_tv_nsec, long end_tv_sec, long end_ntv_sec);

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int gcm_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aad,
	int aad_len, unsigned char *key, int key_len_in_bytes, unsigned char *iv,
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
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

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
	int aad_len, unsigned char *tag, unsigned char *key, int key_len_in_bytes, unsigned char *iv,
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
	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
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
	if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	/* Set expected tag value. Works in OpenSSL 1.0.1d and later */
	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
		handleErrors();

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
	
	unsigned char outbuf[1024];
	unsigned char tag[32];

	gcm_encrypt(gcm_pt, sizeof(gcm_pt), gcm_aad,
			sizeof(gcm_aad), gcm_key, sizeof(gcm_key), gcm_iv,
			outbuf, tag);	
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

void printResult(int total_size, int chunk_size, int key_len_in_bytes, double interval_sec) {


	printf("OpenSSL GCM Encryption(Total=%dMB, key=%dbits, Chunk=%3d%s)\tthroughput= %.2fMB/s.\n", 
			total_size/(1024*1024), 
			key_len_in_bytes * 8, 
			chunk_size>=1024*1024 ? chunk_size/(1024*1024) : chunk_size/1024, 
			chunk_size>=1024*1024 ? "MB" : "KB",
			(double)total_size/(1024*1024*interval_sec) );
}

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
			printResult(total_size, chunk_size_arr[i], key_len_arr[k], interval_sec/itr_num);
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
	test_gcm();
	warm_up();

	gcm_throughput();

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
