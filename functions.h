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
int trialDiv(char *, FILE *) ;
int millerRabin(char *, long long unsigned, FILE *, int ) ;
BIGNUM *RndOddNum(long long unsigned numbits, BIGNUM *num, FILE *) ;
void rndsearch(long long unsigned, long long unsigned, FILE *, FILE *) ;
