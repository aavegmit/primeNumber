#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

using namespace std ;


// Indexed from 0
// Read a bit from the array
unsigned char readBit(unsigned char *str, int location){
	int byte_loc = location / 8  ;
	int bit_loc = location % 8 ;

	unsigned char temp = str[byte_loc] ;

	temp = temp << bit_loc ;
	temp = temp >> 7 ;
	return temp ;

}

// Write a bit at the specified memory location
void writeBit(unsigned char *str, int location, unsigned char value){
	int byte_loc = location / 8  ;
	int bit_loc = location % 8 ;

	unsigned char temp = 0x80 ;
	temp = temp >> bit_loc ;

	if(value == 0x01){
		str[byte_loc] = str[byte_loc] | temp ;
	}
	else{
		str[byte_loc] = str[byte_loc] & ~temp ;
	}


}

void generatePrimes(long long unsigned int maxval){
	maxval = htonl(maxval) ;
	// Create a bitvector of length maxva/8 bytes long
	unsigned char *bitvector = (unsigned char *)malloc(maxval/8  + 2) ;
	memset(bitvector, 0, maxval/8 + 1) ;

	// Call the eratosthenes method to set the respective bit for a non prime number
	bitvector = eratosthenes(maxval, bitvector) ;

	uint32_t out ;
	out = maxval ;
	out = htonl(out) ;
	fwrite(&out, 4, 1, stdout) ;

	// Loop through the bitvector and output the prime numbers on stdout
	for(long long unsigned int i = 2 ; i <= maxval ; ++i){
		if(readBit(bitvector, i) == 0x01){
			out = i ;
			out = htonl(out) ;
			fwrite(&out, 4, 1, stdout) ;
		}
	}

	// Free up the bit vector
	free(bitvector) ;

}

unsigned char *eratosthenes(long long unsigned int maxval, unsigned char *bitvector){
	writeBit(bitvector, 1, 0x00) ;
	for(long long unsigned int i = 2 ; i <= maxval ; ++i)
		writeBit(bitvector, i, 0x01) ;

	long long unsigned int p = 2 , j;


	while(pow(p,2) <= maxval){
		j = pow(p,2) ;
		while( j <= maxval){
			writeBit(bitvector, j, 0x00) ;
			j = j + p ;
		}
		++p ;
		while(readBit(bitvector, p) != 0x01){
			++p ;
		}

	}


	return bitvector ;
}


// Method to check the prime file for 
// correct file format
int checkPrimesFile(FILE *fp){
	uint32_t maxval, dump, temp ;
	fread((uint32_t *)&maxval, 1, 4, fp) ;
	maxval = htonl(maxval) ;
	temp = 1 ;
	while(!feof(fp)){
		int numbytes = fread((uint32_t *)&dump, 1, 4, fp) ;
		dump = htonl(dump) ;
		if(numbytes == 0){
			break ;
		}
		if(numbytes > 0 && numbytes < 4){
			fprintf(stderr, "Primefile size not a multiple of 4\n") ;
			return -1 ;
		}
		if (dump <= temp){
			fprintf(stderr, "Numbers not in ascending order\n") ;
			return -1 ;
		}
		if (dump > maxval){
			fprintf(stderr, "Numbers cannot be greator that maxval value\n") ;
			return -1 ;
		}
		temp = dump ;
	}
	return 1 ;

}

void trialDiv(char *num, FILE *fp){
	// Check the prime file
	if (checkPrimesFile(fp) == -1)
		return ;

	// Create the bignum object
	BIGNUM *bn_num = NULL ;
	BIGNUM *bn_word = NULL ;
	BN_CTX *bn_ctx = NULL ;
	BIGNUM *bn_rem = NULL ;

	bn_num = BN_new() ;
	bn_word = BN_new() ;
	bn_ctx = BN_CTX_new() ;
	bn_rem = BN_new() ;

	if (bn_num == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return ;
	}
	if (bn_word == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return ;
	}
	if (bn_ctx == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return ;
	}
	if (bn_rem == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return ;
	}

	BN_zero(bn_num) ;
	BN_zero(bn_word) ;
	BN_zero(bn_rem) ;
	BN_CTX_init(bn_ctx) ;

	if(BN_dec2bn(&bn_num,  num) == 0){
		fprintf(stderr, "BIGNUM conversion of binary to bn\n") ;
		BN_free(bn_num) ;
		BN_free(bn_word) ;
		BN_free(bn_rem) ;
		return;
	}

	fseek(fp, 0, SEEK_SET) ;

	uint32_t maxval, word ;
	fread((uint32_t *)&maxval, 1, 4, fp) ;
	maxval = htonl(maxval) ;

	while(!feof(fp)){
		int numbytes = fread((uint32_t *)&word, 1, 4, fp) ;
		if(numbytes == 0)
			break ;


		if(BN_bin2bn( (unsigned char *)&word, 4, bn_word) == NULL){
			fprintf(stderr, "BIGNUM conversion of binary to bn\n") ;
			BN_free(bn_num) ;
			BN_free(bn_word) ;
			BN_free(bn_rem) ;
			return;
		}
		if(!BN_cmp(bn_word, bn_num)){
			printf("n passes trial division test\n") ;
			BN_free(bn_num) ;
			BN_free(bn_word) ;
			BN_free(bn_rem) ;
			return ;
		}	

		// Find the mod
		if(BN_mod(bn_rem, bn_num, bn_word, bn_ctx )){
			if ( BN_is_zero(bn_rem)){
				printf("n is composite by trial division (mod %d = 0)\n", htonl(word)) ;
				BN_free(bn_num) ;
				BN_free(bn_word) ;
				BN_free(bn_rem) ;
				return ;
			}
		}


	}

	if ( maxval < (unsigned int)sqrt(atoll(num))){
		printf("n passes trial division test (not enough primes)\n") ;
	}
	else{
		printf("n passes trial division test\n") ;
	}




	BN_free(bn_num) ;
	BN_free(bn_word) ;
	BN_free(bn_rem) ;



}
