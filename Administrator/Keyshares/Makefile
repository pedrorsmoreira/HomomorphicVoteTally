CC = gcc
CFLAGS = -std=c99 -pedantic -Wall

keyshares: aux keyshares.o
	${CC} ${CFLAGS} -o keyshares keyshares.o sss/randombytes/randombytes.o sss/sss.o sss/tweetnacl.o sss/hazmat.o

aux:
	cd sss/randombytes && make && cd .. && make

keyshares.o: keyshares.c sss/sss.h sss/randombytes/randombytes.h 
	${CC} ${CFLAGS} -c keyshares.c

clean:
	rm -f keyshares *keyshares.o && cd sss && make clean && cd randombytes && make clean