CC=gcc
CFLAGS=-Wall -g
DEPS=rsa.h
OBJ=rsa.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rsa: main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

.PHONY: clean
clean:
	rm -f *.o rsa
