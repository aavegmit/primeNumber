#include <math.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <openssl/bn.h>
#include <math.h>

#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))



unsigned char readBit(unsigned char *, int) ;
void writeBit(unsigned char *, int, unsigned char) ;
unsigned char *eratosthenes(long long unsigned int maxval, unsigned char *bitvector) ;
void generatePrimes(long long unsigned int maxva) ;
int checkPrimesFile(FILE *fp) ;
int trialDiv(char *, FILE *) ;
int millerRabin(char *, long long unsigned, FILE *, int ) ;
BIGNUM *RndOddNum(long long unsigned numbits, BIGNUM *num, FILE *) ;
void rndsearch(long long unsigned, long long unsigned, FILE *, FILE *) ;
BIGNUM *maurer(long long unsigned, FILE *, FILE *, int level) ;
unsigned char RndByte(FILE *) ;
