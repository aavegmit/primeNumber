#include <math.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <openssl/bn.h>
#include <math.h>


unsigned char readBit(unsigned char *, int) ;
void writeBit(unsigned char *, int, unsigned char) ;
unsigned char *eratosthenes(long long unsigned int maxval, unsigned char *bitvector) ;
void generatePrimes(long long unsigned int maxva) ;
int checkPrimesFile(FILE *fp) ;
void trialDiv(char *, FILE *) ;
