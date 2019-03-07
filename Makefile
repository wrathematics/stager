MPICC = mpicc
CFLAGS = -std=c99 -O2 -march=native -Wall -pedantic -Wextra


all: clean stager cleaner

%.o: %.c $(DEPS)
	$(MPICC) -c -o $@ $< $(CFLAGS)

stager: src/utils.o src/stager.o
	$(MPICC) -o stager src/utils.o src/stager.o

cleaner: src/utils.o src/cleaner.o
	$(MPICC) -o cleaner src/utils.o src/cleaner.o

clean:
	rm -rf stager cleaner src/*.o
