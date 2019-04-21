CC=gcc
CFLAGS=-fopenmp -g
DEPS=random.h
OBJ=random.o

all: pi_mc pi_mc_p 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pi_mc: pi_mc.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

pi_mc_p: pi_mc_p.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o pi_mc pi_mc_p
