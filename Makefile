CC ?= gcc
CFLAGS = -std=c99 -pedantic -Wall -O3
#CFLAGS = -g -O0

# Diagnostic options
#CFLAGS += -DNOPRINT

# Hardware compilation options
# Enable these CFLAGS for statically allocated arrays
#CFLAGS += -DNOMALLOC
# Enable these CFLAGS to avoid arrays as function arguments
#CFLAGS += -DNOARRIF

EXE = .exe

all: permgen$(EXE)

permgen$(EXE): permgen.o
	$(CC) permgen.o -o permgen$(EXE)

permgen.o: permgen.c
	$(CC) $(CFLAGS) -c permgen.c

tidy:
	rm -f *.o

clean:
	rm -f *.o permgen$(EXE)
