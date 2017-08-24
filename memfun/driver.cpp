#include <cstdlib>
#include <cstring>
#include <ctime>

#include <string>

using namespace std;

//#define memcpy  _i_memcpy_256_unaligned
#ifdef __cplusplus
extern "C" {
#endif

extern void * _i_memcpy_256_unaligned(char* di, const char* si, const int dx);

//extern void *__memcpy_ssse3(char* di, const char* si, size_t __n);

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

bool verify(const char* src, const char* des, 
        const int offset1, const int offset2, const int len,
        const int origSize) {
    for (int i = 0; i< origSize; i++) {
        if( i < offset2 || i >= (offset2 +len)) {
            if ((*(des + i) != '\0')) {
                printf("Copy %d . Error at %d, should be \\0, but %c\n", len, i, *(des + i));
                return false;
            }
        }        
        if ( i>=offset2 && i< (offset2 + len)){ // compare
            if (*(src + offset1 + i-offset2) != *(des + i)) {
                printf("Copy %d . Error at %d, should be %c, but %c\n", len,
                    i, *(src + offset1 + i), *(des + offset2 + i));
                return false;
            }
        }
    }
    return true;
    //printf("Copy %d Successful!\n", len);
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
        srand(std::time(0)); // use current time as seed for random generator
        int offset1 = rand() % (origSize / 2);
        int offset2 = rand() % (origSize / 2);
        
        const char* src = s1 + offset1; 
        char* des = s2 + offset2; 
        
        int len = rand() % (origSize / 2);       
        for(int i=0; i < (origSize / 2); i++) {
            memset(s2, '\0', origSize);
            _i_memcpy_256_unaligned(des, src, i);
            verify(s1, s2, offset1, offset2, i, origSize);
        }  
        printf("Successful!\n");
    }
        
    {
        int offset1 = 0;
        int offset2 = 0;
        const char* src = s1 + offset1;
        char* des = s2 + offset2; 
        
        // break __memcpy_avx_unaligned
        memset(s2, '\0', origSize);
        
        memcpy(des, src, 1);             
        memcpy(des, src, 254);
        memcpy(des, src, 511);
        memcpy(des, src, 2047);
        //memcpy@@GLIBC_2.14
        
        //__memcpy_ssse3(des, src, 8193);
    }
        
    free(s2);
    free(s1);
}
