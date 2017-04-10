/********************************
 * FILENAME: main.c             *
 * DESCRIPTION:                 *
 *		Entry point for project *
 * AUTHOR: Spencer Kitchen      *
 ********************************/

//==============================================================================
//= INCLUDES ===================================================================
//==============================================================================
#include <iostream>
#include <cstdlib>          // rand, srand
#include <ctime>            // time
#include <fstream>          // for file I/O
#include "millerRabin.h"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <climits>
#include <limits>
#include <algorithm>        // reverse

using namespace std;
//==============================================================================
//= DECLARATIONS ===============================================================
//==============================================================================
int KeyGeneration(int randNum);
int encrypt();
void decrypt();
struct KEY getPublicKey (int &error);
struct KEY getPrivateKey (int &error);
struct CIPHER encryptBlock (unsigned long e1, unsigned long e2, unsigned long p,unsigned long P);
string decryptBlock (unsigned long d, unsigned long p, unsigned long c1, unsigned long c2);

struct KEY{
    unsigned long p;
    unsigned long e1;
    unsigned long d;
    unsigned long e2;
};

struct CIPHER{
    unsigned long c1;
    unsigned long c2;
};

//==============================================================================
//= MAIN =======================================================================
//==============================================================================
int main (int argc, const char * argv[]) { 
    bool quit = false;      // controls execution of program

    cout << "\nPublic-Key Cryptosystem!\n\n";
    cout << "Start by generating keys if you haven't done so yet!\n";
    cout << "After, you may encrypt and decrypt.\n\n";

    while (!quit)
    {
        cout << "     CHOOSE SELECTION     " << "\n";
        cout << "==========================" << "\n";
        cout << "Enter number:             " << "\n";
        cout << "   1) Key Generation      " << "\n";
        cout << "   2) Encryption          " << "\n";
        cout << "   3) Decryption          " << "\n";
        cout << "   4) Quit Program        " << "\n";
        int input;
        cout << "> ";
        cin >> input;

        // Makes sure only integers where entered
        while (!cin){
            cin.clear();
            cin.ignore(100, '\n');
            cout << "ERROR: Please enter integers only!\n";
        }

        switch (input)
        {
            case 1:
                int seed;
                cout << "SELECTION: Key Generation\n";
                cout << "ENTER random number for seed\n";
                cout << "> ";
                cin >> seed;

                while (!cin){
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "ERROR: Please enter integers only!\n";
                    cout << "ENTER random number for seed\n";
                    cout << "> ";
                    cin >> seed;
                }

                cout << "generating Keys.........\n\n";
                bool created;
                created = KeyGeneration(seed);
                if (created) {
                    cout << "\nFILE: 'pubkey.txt' created!\n";
                    cout << "FILE: 'prikey.txt' created!\n";
                }
                cout << "\n";
                break;
            case 2:
                cout << "SELECTION: Encryption\n\n";
                bool complete;
                complete = encrypt();
                if (complete) {
                    cout << "\nENCRYPTION: Done!\n";
                    cout << "FILE: 'ctext.txt' created!\n";
                }
                cout << "\n";
                break;
            case 3:
                cout << "SELECTION: Decryption\n\n";
                decrypt();
                break;
            case 4:
                cout << "SELECTION: Quit Program\n";
                cout << "Goodbye!\n\n";
                exit (1);
            default:
                cout << "Unknown Command! Try again...\n\n";
                break;
        }
    }
    return 0;
}
//==============================================================================
//= FUNCTIONS ==================================================================
//==============================================================================

/************************************************************************
 * Key Generation function - Pass it a random number to seed the random *
 * number generator. Creates two files, pubkey.txt & prikey.txt.        *
 * Returns 1 if successful or -1 if failed.                             * 
 ************************************************************************/
int KeyGeneration(int randNum){
    // Start by seeding random number generator
    std::srand(randNum);

    bool foundP = false;
    unsigned long p = 0;
    while (!foundP){
        // generate q with miller millerRabin
        bool foundQ = false;
        unsigned long q = generateSafePrime();  // initialize 
        while (!foundQ){
            q = generateSafePrime();
            // Check if q % 12 = 5
            if(PRINT_DEBUG) cout << "checking (q%12=5) for " << q <<": ";
            if ((q % 12) == 5){
                if(PRINT_DEBUG) cout << "pass\n";
                foundQ = true;
            } else if (PRINT_DEBUG) cout << "fail\n";
        }
        // compute p = 2q + 1 and test if p is prime.
        p = 2*q + 1;
        if(PRINT_DEBUG) cout << "found p: " << p << "\n";
        if(PRINT_DEBUG) cout << "checking p.isPrime: ";
        if (isPrime(p,10)){
             //std::cout << "p: " << p << "\n";
             //std::cout << "p is prime" << "\n";
             if(PRINT_DEBUG) cout << "pass\n";
             foundP = true;
        } else if(PRINT_DEBUG) cout << "fail\n";
    }

    // g is defined as 2 since we built prime off generator 2
    unsigned long g = 2;  

    // find d - picked at random, 0 < d < p 
    unsigned long d = randNumber(p-1,1);
    if(PRINT_DEBUG) cout << "Secret d: " << d << "\n";

    // Compute e2 = g^d % p 
    unsigned long e2 = modMath(g,d,p);

    /***********************
     * PUBLIC KEY - p g e2 *
     ***********************/
    cout << "PUBLIC KEY:\n";
    cout << "p: " << p << " g: " << g << " e2: " << e2 << "\n";
    // Output Keys to file
    ofstream file;
    file.open("pubkey.txt");
    if (file.is_open()){
        file << p << " " << g << " " << e2 << "\n";
        file.close();
        //return 1;
    } else {
        cout << "ERROR: Could not open pubkey.txt\n";
        return 0;
    }

    /***********************
     * PRIVATE KEY - p g d *
     ***********************/
    cout << "PRIVATE KEY:\n";
    cout << "p: " << p << " g: " << g << " d: " << d << "\n";
    // Output Keys to file
    file.open("prikey.txt");
    if (file.is_open()){
        file << p << " " << g << " " << d << "\n";
        file.close();
    } else {
        cout << "ERROR: Could not open prikey.txt\n";
        return 0;
    }
    return 1;
}


/********************************************
 * Encryption Function - get the public key *
 * from file pubkey.txt and plain text from * 
 * ptext.txt. Encrypt block by block and    *
 * output to file ctext.txt.                *
 ********************************************/
int encrypt(){
    int errorno = 0;                            // simple error check
    struct KEY key = getPublicKey(errorno);
    if (errorno){
        //cout << "errorno: " << errorno << "\n";
        //cout << "could not find public key file!\n\n";
        return 0;
    }else {
        cout << "FOUND: 'pubkey.txt': ";
        cout << key.p << " " << key.e1 << " " << key.e2 << "\n";
        cout << "\nENCRYPTED FILE:\n";
    }

    ifstream file;
    if (!errorno) {
        // Get P palintext    
        file.open("ptext.txt");
    }

    if (file.is_open()){
        // open output file
        ofstream of;
        of.open("ctext.txt");

        while (!file.eof()){
            char ch[4] = "";                // holds 8bits * 4 = 32 bits
            file.read(ch, sizeof(ch));      // read 32 bits, top bit is always 0 so really 31 bits.

            string chString(ch);            // convert to string
            // Convert to ascii number
            unsigned long converted = 0;
            for (size_t i = 0; i < min(sizeof(unsigned long), chString.size()); ++i){
                converted <<= CHAR_BIT;
                converted += (unsigned char) chString[i];
            }

            struct CIPHER encryptedBlock = encryptBlock(key.p, key.e1, key.e2, converted);
            //cout << "encrypted block : " << encryptedBlock.c1 << " " << encryptedBlock.c2 << "\n";

            // Save to output file
            if (of.is_open()){
                of << encryptedBlock.c1 << " " << encryptedBlock.c2 << "\n";
                cout << encryptedBlock.c1 << " " << encryptedBlock.c2 << "\n";
            } 
        }
        file.close();
        of.close();
    } 
    return 1;
}


/*******************************************
 * Decrypt function                        *
 *******************************************/
void decrypt(){
    int errorno = 0;                   // simple error check
    struct KEY key = getPrivateKey(errorno);
    if (errorno){
        //cout << "errorno: " << errorno << "\n";
        //cout << "could not find private key file!\n";
    }else {
        cout << "FOUND: 'prikey.txt': ";
        cout << key.p << " " << key.e1 << " " << key.d << "\n";
        cout << "\nDECRYPTED FILE:\n";
    }

    // get c1 and c2
    ifstream file;
    string line;
    file.open("ctext.txt");
    if (file.is_open()){

        // open output file
        ofstream of;
        of.open("dtext.txt");

        while (getline(file,line)){
            string buf;
            stringstream ss(line);
            vector<string> tok;
            while (ss >> buf){
                tok.push_back(buf);
            }
            // convert to unsigned longs
            stringstream ss0(tok[0]);
            stringstream ss1(tok[1]);
            
            // Save c1 and c2
            struct CIPHER encryptedBlock;
            ss0 >> encryptedBlock.c1;
            ss1 >> encryptedBlock.c2;

            //cout << "c1: " << encryptedBlock.c1 << " " << "c2: " << encryptedBlock.c2 << "\n";

            // decrypt block
            string plaintext = decryptBlock(key.d, key.p, encryptedBlock.c1, encryptedBlock.c2);

            cout << plaintext;

            // Save to output file
            if (of.is_open()){
                of << plaintext;
            } 

        }
        file.close();
        cout << "\n\n"; // for spacing in console
        of << "\n";
        of.close();
    } else {
        cout << "ERROR: could not locate 'ctext.txt'\n";
        cout << "Try encrypting something first!\n\n";
    }
}

/*******************************************
 * getPublicKey will parse pubkey.txt and  *
 * return public key.
 *******************************************/
struct KEY getPublicKey (int &error){
    struct KEY pubKey;
    string line;
    ifstream file;
    file.open("pubkey.txt");

    if (file.is_open()){
        while (getline(file,line)){
            string buf;
            stringstream ss(line);
            vector<string> tok;
            while (ss >> buf){
                tok.push_back(buf);
            }
            // Convert to unsigned longs
            stringstream ss0(tok[0]);
            stringstream ss1(tok[1]);
            stringstream ss2(tok[2]);

            // Save to key
            ss0 >> pubKey.p;
            ss1 >> pubKey.e1;
            ss2 >> pubKey.e2;
        }
        file.close();
    } else { 
        cout << "ERROR: Unable to find 'pubkey.txt', try generating keys first!\n\n";
        error = 1;
    }
    return pubKey;
}

/*******************************************
 * getPrivateKey will parse prikey.txt and  *
 * return private key.
 *******************************************/
struct KEY getPrivateKey (int &error){
    struct KEY priKey;
    string line;
    ifstream file;
    file.open("prikey.txt");

    if (file.is_open()){
        while (getline(file,line)){
            string buf;
            stringstream ss(line);
            vector<string> tok;
            while (ss >> buf){
                tok.push_back(buf);
            }
            // Convert to unsigned longs
            stringstream ss0(tok[0]);
            stringstream ss1(tok[1]);
            stringstream ss2(tok[2]);

            // Save to key
            ss0 >> priKey.p;
            ss1 >> priKey.e1;
            ss2 >> priKey.d;
        }
        file.close();
    } else { 
        cout << "ERROR: Unable to find 'prikey.txt', try generating keys first!\n\n";
        error = 1;
    }
    return priKey;
}

/*********************************************
 * encryptBlock takes in a block of text and *
 * the public key and encrypts the block.    *
 * Returns c1 & c2.                          *
 *********************************************/
struct CIPHER encryptBlock (unsigned long p, unsigned long e1, unsigned long e2,unsigned long P){
    //cout << "from encryptBlock\n";
    //cout << "e1: " << e1 << " e2: " << e2 << " p: " << p << " P|" << P << "|\n";

    unsigned long k = randNumber(p-1, 1);
    //cout << "rand k = " << k << "\n";
    
    struct CIPHER c;
    c.c1 = modMath(e1,k,p);
    c.c2 = (modMath(e2,k,p) * modMath(P,1,p)) % p;
    //cout << "c1: " << c.c1 << "\n";
    //cout << "c2: " << c.c2 << "\n";

    return c;
}

/*********************************************
 * decryptBlock takes in private key and     *
 * encrypted text c1 and c2. Returns the     *
 * plaintext for block.                      *
 *********************************************/
string decryptBlock (unsigned long d, unsigned long p, unsigned long c1, unsigned long c2){
    unsigned long m = (modMath(c1, p-1-d, p) * modMath(c2, 1, p)) % p;
    //cout << "m: " << m << "\n";

    // Will mask the lower byte of the number
    const unsigned long mask = numeric_limits<unsigned char>::max();
    string plaintext;
    for (; m > 0; m >>= CHAR_BIT) {
        plaintext += (char) (m & mask);
    }
    // Since bytes where extracted right to left, reverse the text
    reverse(plaintext.begin(), plaintext.end());

    //cout << "converted plaintext\n";
    //cout << plaintext << "\n";

    return plaintext;
}
