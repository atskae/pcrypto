CC=gcc
CFLAGS=-fopenmp -g

all: pi pip

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pi: pi.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

pip: pip.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o pi pip
