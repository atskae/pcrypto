CC=gcc
CFLAGS=-fopenmp -g

all: linked linkedp linkedt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

linked: linked.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

linkedp: linkedp.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

linkedt: linkedt.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o linked linkedp linkedt
