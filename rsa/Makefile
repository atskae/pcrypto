CC= gcc
CFLAGS= -Wall -g -fopenmp -std=c99
DEPS= rsa.h utils.h
OBJ= rsa.o utils.o
LIBS= -lgmp -lm

all: rsa test memcheck

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rsa: main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test: test.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

memcheck: memcheck.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o rsa test memcheck
