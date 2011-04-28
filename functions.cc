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
	if(maxval > pow(2, 24) || maxval < 2){
		fprintf(stderr, "Maxval out of range\n") ;
		exit(0) ;
	}
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

int trialDiv(char *num, FILE *fp){

	fseek(fp, 0, SEEK_SET) ;

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
		return -1;
	}
	if (bn_word == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_ctx == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_rem == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}

	BN_zero(bn_num) ;
	BN_zero(bn_word) ;
	BN_zero(bn_rem) ;
	BN_CTX_init(bn_ctx) ;

	if(BN_dec2bn(&bn_num,  num) == 0){
		fprintf(stderr, "Invalid number\n") ;
		BN_free(bn_num) ;
		BN_free(bn_word) ;
		BN_free(bn_rem) ;
		return -1;
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
			return -1;
		}
		if(!BN_cmp(bn_word, bn_num)){
			printf("n passes trial division test\n") ;
			BN_free(bn_num) ;
			BN_free(bn_word) ;
			BN_free(bn_rem) ;
			return 1;
		}	

		// Find the mod
		if(BN_mod(bn_rem, bn_num, bn_word, bn_ctx )){
			if ( BN_is_zero(bn_rem)){
				printf("n is composite by trial division (mod %d = 0)\n", htonl(word)) ;
				BN_free(bn_num) ;
				BN_free(bn_word) ;
				BN_free(bn_rem) ;
				return 0;
			}
		}


	}

	if ( maxval < (unsigned int)sqrt(atoll(num))){
		printf("n passes trial division test (not enough primes)\n") ;
		BN_free(bn_num) ;
		BN_free(bn_word) ;
		BN_free(bn_rem) ;
		return 2 ;
	}
	else{
		printf("n passes trial division test\n") ;
		BN_free(bn_num) ;
		BN_free(bn_word) ;
		BN_free(bn_rem) ;
		return 1 ;
	}







}

int millerRabin(char *num, long long unsigned maxitr, FILE *fp, int flag){

	BIGNUM *bn_num = NULL ;
	BIGNUM *bn_nminus1 = NULL ;
	BIGNUM *bn_word = NULL ;
	BIGNUM *bn_r = NULL ;
	BN_CTX *bn_ctx = NULL ;
	BIGNUM *bn_rem = NULL ;
	BIGNUM *bn_one = NULL ;
	BIGNUM *bn_two = NULL ;

	bn_num = BN_new() ;
	bn_nminus1 = BN_new() ;
	bn_word = BN_new() ;
	bn_r = BN_new() ;
	bn_ctx = BN_CTX_new() ;
	bn_rem = BN_new() ;
	bn_one = BN_new() ;
	bn_two = BN_new() ;

	if (bn_num == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1 ;
	}
	if (bn_word == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_ctx == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_rem == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_nminus1 == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_r == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_one == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}
	if (bn_two == NULL){
		fprintf(stderr, "BIGNUM new failed\n") ;
		return -1;
	}

	BN_zero(bn_num) ;
	BN_zero(bn_nminus1) ;
	BN_zero(bn_word) ;	// used as a
	BN_zero(bn_rem) ;	// used as y
	BN_zero(bn_r) ;		// used as r
	BN_CTX_init(bn_ctx) ;
	BN_zero(bn_one) ;
	BN_zero(bn_two) ;

	if(BN_dec2bn(&bn_num,  num) == 0){
		fprintf(stderr, "Invalid nuber\n") ;
		BN_free(bn_num) ;
		BN_free(bn_word) ;
		BN_free(bn_rem) ;
		return -1;
	}

	fseek(fp, 0, SEEK_SET) ;

	uint32_t maxval, word ;
	fread((uint32_t *)&maxval, 1, 4, fp) ;
	maxval = htonl(maxval) ;

	if(flag)
		printf("n = %s\n", num) ;
	// Find n-1
	BN_set_word(bn_word, 1) ;
	BN_set_word(bn_one, 1) ;
	BN_set_word(bn_two, 2) ;

	BN_sub(bn_nminus1, bn_num, bn_word) ; 
	if(!flag)
		printf("  ") ;
	printf("  n-1 = %s\n", BN_bn2dec(bn_nminus1 )) ;

	// Find 2^s * r
	long unsigned int s=0 ;
	unsigned char *nminus1 = (unsigned char *)malloc(BN_num_bytes(bn_nminus1))  ;
	BN_bn2bin(bn_nminus1, nminus1) ;
	for(long long int i = 0 ; i < BN_num_bits(bn_nminus1) ;  i++){
		//		if(readBit(nminus1, i) == 0x00){
		if(!BN_is_bit_set(bn_nminus1, i)){
			++s ;
		}
		else
			break ;
	}
	//	--s ;
	if(!flag)
		printf("  ") ;
	printf("  s = %lu\n", s) ;
	if ( s > 0){
		BN_rshift(bn_r, bn_nminus1, s) ;
	}
	if(!flag)
		printf("  ") ;
	printf("  r = %s\n", BN_bn2dec(bn_r)) ;

	for(long long unsigned i = 1 ; i <= maxitr; ++i){
		fread((uint32_t *)&word, 1, 4, fp) ;
		//		word = htonl(word) ;
		BN_bin2bn((unsigned char *)&word, 4, bn_word) ;
		if(BN_cmp(bn_word, bn_nminus1) > 1){
			printf("Miller test failed\n") ;
			return 0;
		}
		BN_mod_exp(bn_rem, bn_word, bn_r, bn_num, bn_ctx) ;
		if(!flag)
			printf("  ") ;
		printf("  Itr %llu of %llu, a = %s, y = %s", i, maxitr, BN_bn2dec(bn_word), BN_bn2dec(bn_rem)) ;
		if(!BN_cmp(bn_rem, bn_nminus1))
			printf(" (which is n-1)") ;
		printf("\n") ;

		if(BN_cmp(bn_rem, bn_one) && BN_cmp(bn_rem, bn_nminus1)){
			for(long unsigned int j=1 ; (j <= s-1) && BN_cmp(bn_rem, bn_nminus1) ; ++j){
				BN_mod_exp(bn_rem, bn_rem, bn_two, bn_num, bn_ctx) ;
				// Printf j and y
				if(!flag)
					printf("  ") ;
				printf("    j = %lu of %lu, y = %s", j, s-1, BN_bn2dec(bn_rem)) ;
				if(!BN_cmp(bn_rem, bn_nminus1))
					printf(" (which is n-1)") ;
				printf("\n") ;
				if(!BN_cmp(bn_rem, bn_one)){
					if(!flag)
						printf("  ") ;
					printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(bn_word)) ;
					return 0;
				}
			}
			if(BN_cmp(bn_rem, bn_nminus1)){
				if(!flag)
					printf("  ") ;
				printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(bn_word)) ;
				//		printf("Composite\n") ;
				return 0;
			}
		}

	}
	if(!flag)
		printf("  ") ;
	printf("Miller-Rabin declares n to be a prime number\n") ;
	return 1;

	}

	BIGNUM *RndOddNum(long long unsigned numbits, BIGNUM *bn_num, FILE *fp){
		long long unsigned numbytes = ceil((double)numbits/8) ;
		unsigned char *word = (unsigned char *)malloc(numbytes) ;
		if (fread(word,1 ,numbytes , fp) != numbytes) 
			exit(0) ;
		BN_bin2bn(word, numbytes, bn_num) ;
		BN_set_bit(bn_num, 0) ;
		BN_set_bit(bn_num, numbits-1) ;
		for (long long unsigned i = numbits ; i < numbytes*8 ; ++i)
			BN_clear_bit(bn_num, i) ;

		free(word) ;

		return bn_num ;
	}

	unsigned char RndByte(FILE *fp){

		unsigned char c ;	
		if (fread(&c,1 ,1 , fp) != 1) 
			exit(0) ;
		return c ;
	}


	void rndsearch(long long unsigned numbits, long long unsigned maxitr, FILE *pfp, FILE *rfp){

		BIGNUM *bn_num = NULL ;

		bn_num = BN_new() ;

		if (bn_num == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return ;
		}
		fseek(pfp, 0, SEEK_SET) ;
		fseek(rfp, 0, SEEK_SET) ;

		//		for(long long unsigned i = 1 ; i <= maxitr ; ++i){
		long long unsigned i = 0 ;
		while(1){
			++i ;
			printf("RANDOM-SEARCH: iteration %llu\n", i) ;
			BN_zero(bn_num) ;
			bn_num = RndOddNum(numbits, bn_num, rfp) ;
			printf("  n = %s\n  ", BN_bn2dec(bn_num)) ;
			if ( trialDiv(BN_bn2dec(bn_num), pfp) == 0 )
				continue ;
			if ( millerRabin(BN_bn2dec(bn_num), maxitr, pfp, 0 ) == 1 )
				return ;

		}

	}



	BIGNUM *maurer(long long unsigned numbits, FILE *pfp, FILE *rfp, int level){
		BIGNUM *bn_num = NULL ;
		BIGNUM *bn_q = NULL ;
		BIGNUM *bn_temp = NULL ;
		BIGNUM *bn_rem = NULL ;
		BIGNUM *bn_I = NULL ;
		BIGNUM *bn_r = NULL ;
		BIGNUM *bn_a = NULL ;
		BIGNUM *bn_b = NULL ;
		BIGNUM *bn_d = NULL ;
		BIGNUM *bn_nminus1 = NULL ;
		BIGNUM *bn_kminus2 = NULL ;
		BN_CTX *bn_ctx = NULL ;

		bn_num = BN_new() ;
		bn_q = BN_new() ;
		bn_I = BN_new() ;
		bn_rem = BN_new() ;
		bn_temp = BN_new() ;
		bn_r = BN_new() ;
		bn_nminus1 = BN_new() ;
		bn_kminus2 = BN_new() ;
		bn_a = BN_new() ;
		bn_b = BN_new() ;
		bn_d = BN_new() ;
		bn_ctx = BN_CTX_new() ;

		if (bn_num == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_q == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_nminus1 == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_kminus2 == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_a == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_ctx == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_r == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_b == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		if (bn_d == NULL){
			fprintf(stderr, "BIGNUM new failed\n") ;
			return NULL;
		}
		BN_CTX_init(bn_ctx) ;

		printf("Maurer: level %d, k=%llu\n", level, numbits) ;

		while (numbits <= 20){
			BN_zero(bn_num) ;
			bn_num = RndOddNum(numbits, bn_num, rfp) ;
			printf("  step 1.1, n = %s\n    ", BN_bn2dec(bn_num)) ;
			if ( trialDiv(BN_bn2dec(bn_num), pfp) != 0 )
				return bn_num ;

		}

		double r = 0.0 ;
		unsigned char rnd ;
		unsigned int m = 20 ;
		printf("  step 4") ;
		if ( numbits <= 2*m)
			r = 0.5 ;
		while( numbits > 2*m){
			rnd = RndByte(rfp) ;
			r = rnd / 255.0 ;
			r = 0.5 + r / 2.0 ;
			if (numbits*(1 - r) > m){
				printf(": random byte = %u", rnd) ;
				break ;
			}
		}
		printf(", r = %d%c\n", round(r*100), 0x25) ;
		bn_q = maurer(floor(r*numbits) + 1, pfp, rfp , level + 1 ) ;
		printf("Maurer: back to level %d, k=%llu, q=%s\n", level, numbits, BN_bn2dec(bn_q)) ;
		long long unsigned int q_num_bits = BN_num_bits(bn_q) ;
		int n_num_bits ;
		// Divide here
//		BN_set_word(bn_temp, pow(2, numbits-2)) ;
		BN_set_word(bn_temp, 2) ;
		BN_set_word(bn_kminus2, numbits - 2) ;
		BN_exp(bn_temp, bn_temp, bn_kminus2, bn_ctx) ;


		BN_div(bn_I, bn_rem, bn_temp, bn_q, bn_ctx) ;
//		printf("I : %s\n\n", BN_bn2dec(bn_I)) ;
		int step7ctr = 0 ;
		// Step 7
		while(1){
			// Step 7.1
			bn_r = RndOddNum(numbits + 1 - q_num_bits, bn_r, rfp) ;
//			BN_mod_add(bn_r, bn_I, bn_r, bn_I, bn_ctx) ;
			BN_mod(bn_r,  bn_r, bn_I, bn_ctx) ;
			BN_add(bn_r, bn_r, bn_I) ;
			BN_add_word(bn_r, 1) ;
			// Compute bn_num
			BN_mul(bn_num, bn_r, bn_q, bn_ctx) ;
			BN_mul_word(bn_num, 2) ;
			BN_add_word(bn_num, 1) ;
			++step7ctr ;
			printf("  step 7, itr %d: R = %s, n = %s\n    ", step7ctr, BN_bn2dec(bn_r), BN_bn2dec(bn_num)) ;
			BN_mul_word(bn_r, 2) ;

			// Step 7.2
			if ( trialDiv(BN_bn2dec(bn_num), pfp) != 0 ){
				n_num_bits = BN_num_bits(bn_num) ;
				bn_a = RndOddNum(n_num_bits, bn_a, rfp) ;
				BN_set_word(bn_temp, 1) ;
				// Step 7.2.1
				BN_copy(bn_nminus1, bn_num) ;
				BN_sub_word(bn_nminus1, 1) ;
				while(BN_cmp(bn_a, bn_temp) <= 0 || BN_cmp(bn_a, bn_nminus1) >=0){
					bn_a = RndOddNum(n_num_bits, bn_a, rfp) ;
				}
				printf("  step 7.2.1, itr %d: a = %s\n", step7ctr,  BN_bn2dec(bn_a)) ;

				// Step 7.2.2
				BN_mod_exp(bn_b, bn_a, bn_nminus1, bn_num, bn_ctx) ;
				BN_set_word(bn_temp, 1) ;
				if(BN_cmp(bn_b, bn_temp) == 0){
					BN_mod_exp(bn_b,  bn_a, bn_r, bn_num, bn_ctx ) ;
					BN_sub_word(bn_b, 1) ;
					BN_gcd(bn_d, bn_b, bn_num, bn_ctx) ;
					if (BN_cmp(bn_d, bn_temp) == 0)
						return bn_num ;
				}


			}



		}

	}
