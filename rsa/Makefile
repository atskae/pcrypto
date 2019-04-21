CC= gcc
CFLAGS= -Wall -g -fopenmp
DEPS= rsa.h utils.h
OBJ= rsa.o utils.o
LIBS= -lgmp -lm

all: rsa 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rsa: main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o rsa 
