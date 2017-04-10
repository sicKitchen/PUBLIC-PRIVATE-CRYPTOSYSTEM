/****************************************
 * FILENAME: millerRabin.h              *
 * DESCRIPTION:                         *
 *		Header file for millerRabin.cpp *
 * AUTHOR: Spencer Kitchen              *
 ****************************************/

#include <stdint.h>
//==============================================================================
#define BLOCKSIZE 31
#define BIT31 1073741824    // constant for '10000...0000' -  31 bits
#define PRINT_DEBUG false    // Turn print statements for key generation on and off

//==============================================================================
unsigned long generateSafePrime();
bool isPrime(unsigned long n, int k);
bool millerRabinTest(unsigned long d, unsigned long n);
unsigned long modMath(unsigned long a, unsigned long d, unsigned long n);
int randNumber(unsigned long max, unsigned long min);
unsigned long get31bitNumber();

