#include <math.h>
#include <stdint.h>
#include <arpa/inet.h>

unsigned char readBit(unsigned char *, int) ;
void writeBit(unsigned char *, int, unsigned char) ;
unsigned char *eratosthenes(long long unsigned int maxval, unsigned char *bitvector) ;
void generatePrimes(long long unsigned int maxva) ;
