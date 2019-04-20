CC=gcc
CFLAGS=-fopenmp -g

all: mandel 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mandel: mandel.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o mandel
