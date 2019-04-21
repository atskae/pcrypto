CC=gcc
CFLAGS=-Wall -g
DEPS=rsa.h rsa_p.h utils.h
OBJ=utils.o
LIBS=-lgmp -lm

all: rsa rsa_p

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rsa: main.o rsa.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

rsa_p: mainp.o rsa_p.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -fopenmp $(LIBS)

.PHONY: clean
clean:
	rm -f *.o rsa rsa_p
