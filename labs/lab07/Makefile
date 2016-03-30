CC   = gcc
EXE  = mytraceroute
CFLAGS    = -Wall -g

all: $(EXE) 

%.o: %.c
	$(CC) -c $(CFLAGS) $<

mytraceroute: traceroute.o 
	$(CC) -o mytraceroute traceroute.o 
clean:
	/bin/rm -rf *~ *.o core $(EXE)