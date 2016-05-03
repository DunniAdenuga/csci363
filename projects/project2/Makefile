CC     = gcc -std=gnu99
CFLAGS = -g
LFLAGS =

HDR    = crcmode.h
SRC    = crcmodel.c crctest.c 
OBJ    = crcmodel.o crctest.o
SOBJ   = source.o
SIOBJ  = sink.o
EXE    = crctest
EXE2 = source  sink

all: $(EXE) $(EXE2)

%.o: %.c %.h
	$(CC) -c $(CLFAGS) $<

$(EXE): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(EXE)

source: $(SOBJ)
	$(CC) $(LFLAGS) $(SOBJ) -o source crcmodel.o

sink: $(SIOBJ)
	$(CC) $(LFLAGS) $(SIOBJ) -o sink crcmodel.o



clean:
	/bin/rm -f *.o *~ core $(EXE) $(EXE2)
