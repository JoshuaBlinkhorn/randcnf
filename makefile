CC=gcc

randcnf: randcnf.o
	$(CC) -o randcnf randcnf.o

clean:
	rm -f randcnf randcnf.o
