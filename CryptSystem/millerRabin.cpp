/****************************************************************
 * FILENAME: millerRabin.cpp                                    *
 * DESCRIPTION:                                                 *
 *		Holds all functions related to using miller-rabin test  *
 * AUTHOR: Spencer Kitchen                                      *
 ****************************************************************/
//==============================================================================
#include <iostream>         // cout
#include <cstdlib>          // rand, srand
#include <ctime>            // time
#include <cmath>            // pow
#include "millerRabin.h"

using namespace std;
//==============================================================================
/********************************************
 * Generates a Safe Prime number that is 31 *
 * bits long.                               * 
 ********************************************/
unsigned long generateSafePrime(){    
    bool found = false;
    unsigned long q = get31bitNumber();
   
    while (!found){ 
        q = get31bitNumber();

        // Simple first test, check if odd
        if (q % 2 != 0) {
            // Now check if q is prime
            if (isPrime(q,10)){
                if (PRINT_DEBUG) cout << "found q: " << q << "\n";
                found = true;
            }
        }
    }
    return q;
}

/*******************************************
 * Check if number is prime                *
 *     Returns false if n is composite     *
 *     Returns true if n is probably prime *
 *   k is how many times to run test as    *
 *   test is not always accurate depending *
 *   on what a is.                         *
 *******************************************/
bool isPrime(unsigned long n, int k){
    // Handle 0-4 number 0-4 manually
    //  1 = composite, 2 = prime, 3 = prime, 4 = composite
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    // find d & r so that they fit the formula
    //      n-1 = d*2^r
    unsigned long d = n - 1;
    int r = 0;
    while (d % 2 == 0){
        d /= 2;
        r++;
    }

    // Pass into miller-rabin test
    for (int i = 0; i < k; i++){
        if (millerRabinTest(d,n) == false){
            return false;
        }
    }
    return true;
}

/********************************************
 * Miller-Rabin Test                        *
 *   Returns false if n is composite        *
 *   Returns true if n is probably prime    *
 *   d is a odd number, r >= 1              *
 *   d & r fit equation n-1 = d*2^r         *
 ********************************************/
bool millerRabinTest(unsigned long d, unsigned long n){
    // Pick a random number a between 1 < a < n-1
    unsigned long a = randNumber(n-2, 2);

    // Compute x = a^d % m
    unsigned long x = modMath(a, d, n);
    if (x == 1 || x == n-1){
        return true;
    }

    // Square x until one of the conditions are meet
    // d does not reach n-1
    // x^2 % n is not 1
    // x^2 % n is not n-1
    while (d != n-1){
        x = (x*x) % n;
        d *= 2;
        if(x == 1) return false;
        if(x == n-1) return true;
    }
    return false;
}

/*******************************************
 * Mod Math                                *
 *   Function does modular exponent math   *
 *   Returns a^d % n                       *
 *******************************************/
unsigned long modMath(unsigned long a, unsigned long d, unsigned long n){
    unsigned long result = 1;
    a = a % n;
    while (d > 0){
        // if d is odd
        if (d % 2 != 0){
            result = (result * a) % n;
        }

        d = d>>1;  // floor of d/2
        a = (a*a) % n; 
    }
    return result;
}

/***********************************************
 * Generates random number between min and max *
 * Normal rand, RAND_MAX = 2147483647          *
 ***********************************************/
int randNumber(unsigned long max, unsigned long min){
    int random = (std::rand() % max) + min;
    return random;
}

/*******************************************
 * Generates a 31 bit random number        *
 *******************************************/
unsigned long get31bitNumber(){
    unsigned long x = BIT31;
    unsigned long r = rand();
    x |= r;
    return x;
}

