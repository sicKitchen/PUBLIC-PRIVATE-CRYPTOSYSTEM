# Public/Private Key Cryptosystem

Cryptosystem is an encryption and decryption program based on public and private keys.

## Description

This program encrypt/decrypt a text file of any length. Since this cipher is based on
modulo exponentiations all input and output files can contain integers. In this case, text is treated
as ASCII. Given a file containing text, the program outputs the ciphertext as a file containing numbers.
The program prompts the user to choose between key generation,encryption or decryption.

**Key generation:**

* Prompt the user for a random number with which to seed a random number generator.

* Output a public-key p g e 2 and the corresponding private key p g d. These keys should be output into two files: pubkey.txt and prikey.txt. The format of these files should be: the first line of the file contains the integer key values and they are only separated by a space, as: p g d. Note: in the class slides e 1 is the generator g.

**Encryption:**

* As input, the program should take as plaintext a standard ASCII text file (call the file ptext.txt) and a public-key p g e 2 contained in the file pubkey.txt.

* Output the ciphertext as pairs of integers C 1 and C 2 to a text file called ctext.txt. So if the message M = m 1 m 2 m 3 ... m n then each m i will give a pair C 1 and C 2 .

**Decryption:**

* As input, the program should take as ciphertext a standard ASCII text file (ctext.txt) containing integer pairs C 1 and C 2 and a private-key p g d contained in the file prikey.txt.

* Output the decryption of the ciphertext into a file called dtext.txt. In the case of decryption, your program should also print the output (plaintext) to the screen.

## Archive

```txt
├── CryptSystem
│   ├── main.cpp            EntryPoint of program containing encrypt/decrypt functions and menu.
│   ├── makefile            Compiles and cleans program and related files.
│   ├── millerRabin.cpp     Miller Rabin Test implementation.
│   ├── millerRabin.h       Header for miller Rabin as well as some definitions.
│   └── ptext.txt           Plain text secret message. Edit this file to change message.
└── README.md

1 directory, 6 files
```

## Building & Installing

* Unpack cs427-program2-SpencerKitchen.tar.gz - will extract a folder called `CryptoSystem`
* Open folder `CryptoSystem` in terminal
* Compiling `cryptosystem` (enter in terminal)

```bash
make
```

* To recompile from scratch (enter in terminal)

```bash
make clean
```

* Running Program (enter in terminal)

```bash
./cryptosystem
```

## Running `cryptosystem`

* When running program the user will be greated with a selection screen.

```txt
     CHOOSE SELECTION
==========================
Enter number:
   1) Key Generation
   2) Encryption
   3) Decryption
   4) Quit Program
>
```

Enter the number of the selection you wish to make.

### Using program

1. First you need to generate public and private keys by SELECTING `1`.

    Creates files `pubtext.txt` and `pritext.txt`.
1. Encrypt `ptext.txt` by SELECTING `2`.

    Creates the file `ctext.txt`.
1. Decrypt `ctext.txt` by SELECTING `3`.

    Creates the file `dtext.txt`.
1. Quit at any time by entering `4`.

### Changing Secret Message

To change the message being encrypted, edit the file `ptext.txt`.

## OUTPUT

The program will output all the relevant information for each selection as described in the Description.

To see the output of the Key Generation part, you can set a DEBUG flag in the code on line 12.

**FILE: `millerRabin.h`**

```c++
#define PRINT_DEBUG false    // Turn print statements for key generation on and off
```

## Built With

* c++11 - Must compile with flag -std=c++11
* GNU Make
* Ubuntu 16.04Lts

## Tested On

* Ubuntu 16.04Lts
* OSX 10.12.3 (Sierra)

## Author

* **Spencer Kitchen**
* email: spencer.kitchen@wsu.edu
