MPICC = mpicc
CFLAGS = -std=c99 -O2 -march=native -Wall -pedantic -Wextra


all: clean stager cleaner

stager:
	$(MPICC) $(CFLAGS) src/stager.c -o stager

cleaner:
	$(MPICC) $(CFLAGS) src/cleaner.c -o cleaner

clean:
	rm -rf stager cleaner
