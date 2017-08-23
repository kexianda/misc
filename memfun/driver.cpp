#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <string>

using namespace std;

//#define memcpy  _i_memcpy_256_unaligned
#ifdef __cplusplus
extern "C" {
#endif

extern char* _i_memcpy_256_unaligned(char* di, const char* si, const int dx);

#ifdef __cplusplus
}
#endif


void testMem();
/*
 *
 */
int main(int argc, char** argv) {

    testMem();
    return 0;
}

void verify(const char* src, const char* des, 
        const int offset1, const int offset2, const int len,
        const int origSize) {
    for (int i = 0; i< origSize; i++) {
        if( i < offset2 || i >= (offset2 +len)) {
            if ((*(des + i) != '\0'))
                printf("Error at %d, should be \\0, but %c", i, *(des + i));
        }        
        else { // compare
            if (*(src + offset1 - offset2 + i) != *(des + i)) {
                printf("Error at %d, should be %c, but %c", 
                    i, *(src + offset1 - offset2 + i), *(des + i));
            }
        }
    }
    printf("Successful!");
}

void testMem() {

//    const auto t1 = std::chrono::high_resolution_clock::now();
//    const auto t2 = std::chrono::high_resolution_clock::now();
//    const std::chrono::duration<double> td = t2 - t1;
//    printf("reference result = %lu, time = %10.6f s\n", result, td.count());

    //int size = 256 + 128 + 64 + 32 + 16 + 9; // 505
    int origSize = 1024 * 8; //two pages
    char* s1 = (char*) malloc(sizeof (char) * origSize);
    char* s2 = (char*) malloc(sizeof (char) * origSize);
    for (int i = 0; i < origSize; i++) 
        s1[i] = ('a' + i%26);
    memset(s2, '\0', origSize);
    
    // not aligned
    {
        int offset1 = 26;
        int offset2 = 5;
        const char* src = s1 + offset1; 
        char* des = s2 + offset2; 
        int len = 256 + 128 + 64 + 32 + 16 + 9; // 505
        _i_memcpy_256_unaligned(des, src, len);
        verify(s1, s2, offset1, offset2, len, origSize);
    }
        
    {
        int offset1 = 0;
        int offset2 = 0;
        const char* src = s1 + offset1;
        char* des = s2 + offset2; 
        
        // break __memcpy_avx_unaligned
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 7);
        memset(s2, '\0', origSize);
        memcpy(des, src, 8);
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 15);
        memset(s2, '\0', origSize);
        memcpy(des, src, 16);
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 31);
        memset(s2, '\0', origSize);        
        memcpy(des, src, 32);
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 63);
        memset(s2, '\0', origSize);        
        memcpy(des, src, 64);
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 127);
        memset(s2, '\0', origSize);        
        memcpy(des, src, 128);
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 254);
        memcpy(des, src, 511);
        memcpy(des, src, 2047);
        memcpy(des, src, 8193);
    }
        
    free(s2);
    free(s1);
}
