CC=g++
NAME= cryptosystem
CFLAGS= -std=c++11 -Wall
DEPS= main.o millerRabin.o millerRabin.h
CPPFILES= main.cpp millerRabin.cpp
TXTFILES= pubkey.txt prikey.txt ctext.txt dtext.txt

cryptosystem: $(DEPS)
	$(CC) $(CFLAGS) $(CPPFILES) -o $(NAME)

clean:
	rm *.o $(NAME) $(TXTFILES)