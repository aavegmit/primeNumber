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
	// Create a bitvector of length maxva/8 bytes long
	unsigned char *bitvector = (unsigned char *)malloc(maxval/8  + 2) ;
	memset(bitvector, 0, maxval/8 + 1) ;
	
	// Call the eratosthenes method to set the respective bit for a non prime number
	bitvector = eratosthenes(maxval, bitvector) ;

	uint32_t out ;
			memcpy(&out, &maxval, 4) ;
			out = htonl(out) ;
			fwrite(&out, 4, 1, stdout) ;

	// Loop through the bitvector and output the prime numbers on stdout
	for(long long unsigned int i = 2 ; i <= maxval ; ++i){
		if(readBit(bitvector, i) == 0x01){
			memcpy(&out, &i, 4) ;
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
