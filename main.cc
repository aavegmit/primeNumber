#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

using namespace std ;


// Usage of the program
void usage(){
	printf("Usage:\n\t ./hw7 primes -n=maxval\n") ;
	printf("\t ./hw7 trialdiv -n=number -p=primesfile\n") ;
	printf("\t ./hw7 millerrabin -n=number -t=maxtir -p=primesfile\n") ;
	printf("\t ./hw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n") ;
	printf("\t ./hw7 maurer -k=numbits -p=primesfile -r=rndfile\n") ;
	exit(0) ;
}

int main(int argc, char **argv){

	int choice;
	char *poly1, *poly2, *tablefile ;
	FILE *inpF, *rndFile ;
	long long unsigned int maxval, maxitr, numbits ;
	char *number ;



	// Parse the command line 
	if (argc < 3){
		usage() ;
	}
	else {
		argv++ ;
		if (strcmp(*argv, "primes") == 0) {
			choice = 1 ;
		} else if (strcmp(*argv, "trialdiv") == 0) {
			choice = 2 ;
		} else if (strcmp(*argv, "millerrabin") == 0) {
			choice = 3 ;
		} else if (strcmp(*argv, "rndsearch") == 0) {
			choice = 4 ;
		} else if (strcmp(*argv, "maurer") == 0) {
			choice = 5 ;
		} else {
			usage() ;
		}

		argv++ ;

		// keygen chosen
		if (choice == 1){
			if (argc < 3){
				usage() ;
			}
			for (int i = 0 ; i < 1 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'n') {
						/* read pphrase from *argv */
						tablefile = strtok(*argv, "=") ;
						tablefile = strtok(NULL, "=") ;
						if (tablefile == NULL)
							usage() ;
						maxval = atoll(tablefile) ;
						if(maxval == 0){
							fprintf(stderr, "Maxval should be greator than zero\n") ;
							exit(0) ;
						}

					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 2){
			if (argc < 4){
				usage() ;
			}
			for (int i = 0 ; i < 2 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'n' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						number = poly1 ;
					} 
					else if (argv[0][1] == 'p'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						inpF = fopen(poly2, "r") ;
						if (inpF == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 3){
			if (argc < 5){
				usage() ;
			}
			for (int i = 0 ; i < 3 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'n' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						number = poly1 ;
					} 
					else if (argv[0][1] == 't' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						maxitr = atoll(poly1) ;
						if(maxitr == 0){
							fprintf(stderr, "Maxitr should be greator than zero\n");
							exit(0) ;
						}
					} 
					else if (argv[0][1] == 'p'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						inpF = fopen(poly2, "r") ;
						if (inpF == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 4){
			if (argc < 6){
				usage() ;
			}
			for (int i = 0 ; i < 4 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						numbits = atoll(poly1) ;
						if(numbits == 0){
							fprintf(stderr, "Numbits should be greator than zero\n");
							exit(0) ;
						}
					} 
					else if (argv[0][1] == 't' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						maxitr = atoll(poly1) ;
						if(maxitr == 0){
							fprintf(stderr, "Maxitr should be greator than zero\n");
							exit(0) ;
						}
					} 
					else if (argv[0][1] == 'p'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						inpF = fopen(poly2, "r") ;
						if (inpF == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else if (argv[0][1] == 'r'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						rndFile = fopen(poly2, "r") ;
						if (rndFile == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}
		else if(choice == 5){
			if (argc < 5){
				usage() ;
			}
			for (int i = 0 ; i < 3 ; i++, argv++) {
				if (*argv[0] == '-') {
					if (argv[0][1] == 'k' ) {
						/* read pphrase from *argv */
						poly1 = strtok(*argv, "=") ;
						poly1 = strtok(NULL, "=") ;
						if (poly1 == NULL)
							usage() ;
						numbits = atoll(poly1) ;
						if(numbits == 0){
							fprintf(stderr, "Numbits should be greator than zero\n");
							exit(0) ;
						}
					} 
					else if (argv[0][1] == 'p'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						inpF = fopen(poly2, "r") ;
						if (inpF == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else if (argv[0][1] == 'r'  ) {
						/* read pphrase from *argv */
						poly2 = strtok(*argv, "=") ;
						poly2 = strtok(NULL, "=") ;
						if (poly2 == NULL)
							usage() ;
						rndFile = fopen(poly2, "r") ;
						if (rndFile == NULL){
							fprintf(stderr, "Primesfile could not be opened\n") ;
							exit(0) ;
						}	       
					} 
					else{
						usage() ;
					}
				}
				else{
					usage() ;	
				}

			}
		}

	}



	// Command line parsing done, now call respective methods

	if (choice == 1){
//		printf("Maxval: %llu\n", maxval) ;
		generatePrimes(maxval) ;
		
	}
	else if (choice == 2){
		trialDiv(number, inpF) ;

		fclose(inpF) ;

	}
	else if (choice == 3){
		printf("number: %s, maxitr: %llu\n", number, maxitr) ;

		fclose(inpF) ;
	}
	else if (choice == 4){
		printf("numbits: %llu, maxitr: %llu", numbits, maxitr) ;

		fclose(inpF) ;
		fclose(rndFile) ;
	}
	else if (choice == 5){
		printf("numbits: %llu\n", numbits) ;

		fclose(inpF) ;
		fclose(rndFile) ;
	}










	return 0;
}
