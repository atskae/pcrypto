CC=gcc
CFLAGS=-fopenmp -g

all: prod_cons prod_cons_p 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prod_cons: prod_cons.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

prod_cons_p: prod_cons_p.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o prod_cons prod_cons_p
