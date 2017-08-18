#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <string>

using namespace std;

//#define memcpy  _i_memcpy_256_unaligned

extern char* _i_memcpy_256_unaligned(char* di, char* si, int dx);
void testMem();
/*
 *
 */
int main(int argc, char** argv) {

    testMem();
    return 0;
}

void testMem() {

//    const auto t1 = std::chrono::high_resolution_clock::now();
//    const auto t2 = std::chrono::high_resolution_clock::now();
//    const std::chrono::duration<double> td = t2 - t1;
//    printf("reference result = %lu, time = %10.6f s\n", result, td.count());

    int size = 256 + 128 + 64 + 32 + 16 + 9;
    char* s1 = (char*)malloc(sizeof(char) * size);
    char* s0 = (char*)malloc(sizeof (char) * (size + 5));
    char* s2 = s0 + 5; //not 16 aligned!
    for(int i =0; i < size; i++)
        s1[i] = 'a' + i%26;

    _i_memcpy_256_unaligned(s2, s1, size);
    for (int i = 0; i < size; i++)
        s2[i] = 0;

    _i_memcpy_256_unaligned(s2, s1, size);
    free(s2);
    free(s1);
}
