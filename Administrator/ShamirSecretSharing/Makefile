CC = gcc
CFLAGS = -O3 -Ofast -std=c99 -pedantic -Wall

default: program

program: aux splitKeyShares.o
	${CC} ${CFLAGS} -o splitKeyShares splitKeyShares.o sss/randombytes/randombytes.o sss/sss.o sss/tweetnacl.o sss/hazmat.o

aux:
	cd sss/randombytes && make && cd .. && make

splitKeyShares.o:
	${CC} ${CFLAGS} -c splitKeyShares.c

clean:
	rm -f *.o splitKeyShares && cd sss/randombytes && make clean && cd .. && make clean
 
###

test: aux2 test.o
	${CC} ${CFLAGS} -o test test.o sss/randombytes/randombytes.o sss/sss.o sss/tweetnacl.o sss/hazmat.o

aux2:
	cd sss/randombytes && make && cd .. && make

test.o:
	${CC} ${CFLAGS} -c test.c

clean-test:
	rm -f *.o test && cd sss/randombytes && make clean && cd .. && make clean

###

testing: aux3 testing.o
	${CC} ${CFLAGS} -o testing testing.o sss/randombytes/randombytes.o sss/sss.o sss/tweetnacl.o sss/hazmat.o

aux3:
	cd sss/randombytes && make && cd .. && make

testing.o:
	${CC} ${CFLAGS} -c testing.c

clean-testing:
	rm -f *.o testing && cd sss/randombytes && make clean && cd .. && make clean