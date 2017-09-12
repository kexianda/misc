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

#ifdef __cplusplus
}
#endif

void debugfun();
void testMem();
void bench();
/*
 *
 */
int main(int argc, char** argv) {
   
    debugfun();

    
#ifdef _RUN_TEST_
    testMem();
#endif

#ifdef _RUN_BENCHMARK_    
    bench();
#endif
    
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

void debugfun() {
    int origSize = 1024 * 1024 *8; //two pages
    char* s1 = (char*) malloc(sizeof (char) * origSize);
    char* s2 = (char*) malloc(sizeof (char) * origSize);
    for (int i = 0; i < origSize; i++)
        s1[i] = ('a' + i % 26);
    memset(s2, '\0', origSize);

    {    
        int offset1 = 0;
        int offset2 = 5;
        
        const char* src = s1 + offset1;
        char* des = s2 + offset2;       
        
        int len = 1024 * 4;
        memset(s2, '\0', origSize);
        _i_memcpy_256_unaligned(des, src, len);
        if(verify(s1, s2, offset1, offset2, len, origSize)){
            printf("Copy %d Successful!\n", len);
        }        
        
        len = 1024*1024*6+13; 
        memset(s2, '\0', origSize);
        memcpy(des, src, len);
        if (verify(s1, s2, offset1, offset2, len, origSize)) {
            printf("Copy %d Successful!\n", len);
        }
        memset(s2, '\0', origSize);
        _i_memcpy_256_unaligned(des, src, len);
        if (verify(s1, s2, offset1, offset2, len, origSize)) {
            printf("Copy %d Successful!\n", len);
        }
        
    }
}

void testMem() {

    int origSize = 1024 * 1024 * 16; 
    char* s1 = (char*) malloc(sizeof (char) * origSize);
    char* s2 = (char*) malloc(sizeof (char) * origSize);
    for (int i = 0; i < origSize; i++) 
        s1[i] = ('a' + i%26);
    memset(s2, '\0', origSize);
    
    // test small buffers
    //TODO: fix it.
//    {
//        int offset1 = rand() % 16;
//        int offset2 = rand() % 16;
//        const char* src = s1 + offset1;
//        char* des = s2 + offset2;
//        
//        int range = 4096*4;
//
//        for (int len = 1; len < 1024; len++) {
//            memset(s2, '\0', range);
//            _i_memcpy_256_unaligned(des, src, len);
//            verify(s1, s2, offset1, offset2, len, range);     
//        }
//    }
        
    for (int i = 0; i < 10; i++) {
        bool suc = true;
        srand(std::time(0));
        int len = rand() % (origSize / 2);
        int maxoffset = 16;

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
        if (suc) printf("Round %d succeed!\n", i);
    }

    free(s2);
    free(s1);
}

void warmup() {
    int size = 1024 * 1024 * 1024;
    char* s1 = (char*) malloc(sizeof (char) * size);
    char* s2 = (char*) malloc(sizeof (char) * size);
    int itrNum = 20;
    printf("%s\n", "warming up ...");

    for (int i = 0; i < size; i++)
        s1[i] = ('a' + i % 26);
    
    int blocksizes[] = {128, 512, 1024, 4096, 1024 * 1024, 1024 * 1024 * 2, 1024 * 1024 * 8 };
    {        
        for (int i = 0; i < sizeof (blocksizes) / sizeof (int); i++) {
            int num = size / blocksizes[i];
            for (int itr = 0; itr < itrNum; itr++) {
                for (int j = 0; j < num; j++) {
                    memcpy((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
                }
            }
        }
    }

    {
        for (int i = 0; i < sizeof (blocksizes) / sizeof (int); i++) {
            int num = size / blocksizes[i];

            for (int itr = 0; itr < itrNum; itr++) {
                for (int j = 0; j < num; j++) {
                    _i_memcpy_256_unaligned((s2 + blocksizes[i] * j), (s1 + blocksizes[i] * j), blocksizes[i]);
                }
	    }
        }
    }
    
    free(s2);
    free(s1);
}

void bench() {
    
    warmup();
    const int itrNum = 50;
    int size = 1024 * 1024 * 1024;
    char* s1 = (char*) malloc(sizeof (char) * size);
    char* s2 = (char*) malloc(sizeof (char) * size);
    for (int i = 0; i < size; i++)
        s1[i] = ('a' + i % 26);
    
    int blocksizes[] = {8, 16, 128, 512, 1024, 
                        4096, 1024 * 1024, 1024 * 1024 * 4, 1024 * 1024 * 8, 
                        73, 113, 1024 * 1024 + 97};
   
    {
        
        for (int i = 0; i < sizeof(blocksizes)/sizeof(int); i++) {
            
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
            printf("glibc,memcpy:\tblockSize=%d, \t\tresult = %10.6f MB/s,\ttime = %10.6f s\n", blocksizes[i], result, td.count()); 
        }
    }

    {
        
        for (int i = 0; i < sizeof (blocksizes) / sizeof (int); i++) {

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
            printf("our poc memcpy:\tblockSize=%d,  \t\tresult = %10.6f MB/s,\ttime = %10.6f s\n", blocksizes[i], result, td.count());
        }         
    }
}
