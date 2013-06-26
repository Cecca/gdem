CC = gcc

MPI_FLAGS = -I/usr/lpp/ppe.poe/include -lmpi -L/usr/lpp/ppe.poe/lib

CFLAGS = -Wall -O2 -std=c99 -DNDEBUG $(MPI_FLAGS)

default: gdem

clean:
	rm -r bin *.o gdem
.PHONY: clean

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

gdem : parallel.o hll_counter.o parser.o graph.o hyper_anf_mpi.o
	$(CC) $(CFLAGS) -o gdem parallel.o hll_counter.o parser.o graph.o hyper_anf_mpi.o


