CC=gcc
CFLAGS=-Wall -g
LIBS=-lgmp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prac: main.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o prac
