#include <cstdlib>
#include <cstring>
#include <ctime>
#include <chrono>

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
void bench();
/*
 *
 */
int main(int argc, char** argv) {

    //testMem();
    bench();
    return 0;
}

bool verify(const char* src, const char* des, 
        const int offset1, const int offset2, const int len,
        const int origSize) {
    for (int i = 0; i< origSize; i++) {
        if( i < offset2 || i >= (offset2 +len)) {
            if ((*(des + i) != '\0')) {
                printf("Copy %d . Error at %d, should be \\0, but %c\n", len, i, *(des + i));
                printf("offset1 %d, offset2 %d, len=%d", offset1, offset2, len);
                return false;
            }
        }        
        if ( i>=offset2 && i< (offset2 + len)){ // compare
            if (*(src + offset1 + i-offset2) != *(des + i)) {
                printf("Copy %d . Error at %d, should be %c, but %c\n", len,
                    i, *(src + offset1 + i), *(des + offset2 + i));
                printf("offset1 %d, offset2 %d, len=%d\n", offset1, offset2, i);
                return false;
            }
        }
    }
    return true;
    //printf("Copy %d Successful!\n", len);
}

void testMem() {

    //int size = 256 + 128 + 64 + 32 + 16 + 9; // 505
    int origSize = 1024 * 8; //two pages
    char* s1 = (char*) malloc(sizeof (char) * origSize);
    char* s2 = (char*) malloc(sizeof (char) * origSize);
    for (int i = 0; i < origSize; i++) 
        s1[i] = ('a' + i%26);
    memset(s2, '\0', origSize);
    
    {
        //Copy 256 . Error at 576, should be \0, but n
        //offset1 537, offset2 320, len = 256offset1 537, offset2 320, len = 256     
        int offset1 = 0;
        int offset2 = 0;
        int len = 256;
        const char* src = s1 + offset1;
        char* des = s2 + offset2;

        memset(s2, '\0', origSize);
        _i_memcpy_256_unaligned(des, src, len);
        verify(s1, s2, offset1, offset2, len, origSize);     
    }
        
    for (int i = 0; i < 100; i++) {
        bool suc = true;
        srand(std::time(0));
        int len = rand() % (origSize / 2);
        int maxoffset = 128;

        for(int offset1=0; offset1 <= maxoffset; offset1++) {
            for (int offset2 = 0; offset2 <= maxoffset; offset2++) {
                const char* src = s1 + offset1;
                char* des = s2 + offset2;

                memset(s2, '\0', origSize);
                _i_memcpy_256_unaligned(des, src, len);
                suc = verify(s1, s2, offset1, offset2, len, origSize);
                if (!suc) break;
            }
        }
        if (suc) printf("successful!\n");
    }

L_END:
    free(s2);
    free(s1);
}

void warmup() {
    int size = 1024 * 1024 * 1024;
    char* s1 = (char*) malloc(sizeof (char) * size);
    char* s2 = (char*) malloc(sizeof (char) * size);
    for (int i = 0; i < size; i++)
        s1[i] = ('a' + i % 26);

    {
        int blocksizes[] = {128, 512, 1024, 4096, 1024 * 1024};
        for (int i = 0; i < 5; i++) {

            const auto t1 = std::chrono::high_resolution_clock::now();
            int num = size / blocksizes[i];
            for (int j = 0; j < num; j++) {
                memcpy((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
            }

            const auto t2 = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> td = t2 - t1;
            double result = 1024.00 / td.count();
            //printf("blockSize=%d,  result = %10.6f MB/s, time = %10.6f s\n", blocksizes[i], result, td.count());
        }
    }

    {
        const auto t1 = std::chrono::high_resolution_clock::now();

        int blocksizes[] = {128, 512, 1024, 4096, 1024 * 1024};
        for (int i = 0; i < 5; i++) {

            const auto t1 = std::chrono::high_resolution_clock::now();
            int num = size / blocksizes[i];
            for (int j = 0; j < num; j++) {
                _i_memcpy_256_unaligned((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
            }

            const auto t2 = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> td = t2 - t1;
            double result = 1024.00 / td.count();
            //printf("blockSize=%d,  result = %10.6f MB/s, time = %10.6f s\n", blocksizes[i], result, td.count());
        }
    }
}
void bench() {
    
    warmup();
    const int itrNum = 20;
    int size = 1024 * 1024 * 1024;
    char* s1 = (char*) malloc(sizeof (char) * size);
    char* s2 = (char*) malloc(sizeof (char) * size);
    for (int i = 0; i < size; i++)
        s1[i] = ('a' + i % 26);
   
    {
        int blocksizes[] = {128, 512, 1024, 4096, 1024 * 1024};
        for (int i = 0; i < 5; i++) {
            
            const auto t1 = std::chrono::high_resolution_clock::now();
            int num = size / blocksizes[i];
            
            for (int itr = 0; itr < itrNum; itr++) {
                for (int j = 0; j < num; j++) {
                    memcpy((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
                }
            }

            const auto t2 = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> td = t2 - t1;
            double result = ((double)itrNum*(size/(1024*1024))) / td.count();
            printf("blockSize=%d,  result = %10.6f MB/s, time = %10.6f s\n", blocksizes[i], result, td.count()); 
        }
    }

    {
        int blocksizes[] = {128, 512, 1024, 4096, 1024 * 1024};
        for (int i = 0; i < 5; i++) {

            const auto t1 = std::chrono::high_resolution_clock::now();
            int num = size / blocksizes[i];

            for (int itr = 0; itr < itrNum; itr++) {
                for (int j = 0; j < num; j++) {
                    _i_memcpy_256_unaligned((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
                }
            }

            const auto t2 = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> td = t2 - t1;
            double result = ((double) itrNum * (size / (1024 * 1024))) / td.count();
            printf("blockSize=%d,  result = %10.6f MB/s, time = %10.6f s\n", blocksizes[i], result, td.count());
        }         
    }
}
