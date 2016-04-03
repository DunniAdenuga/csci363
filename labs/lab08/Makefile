CC     = gcc
CFLAGS = -g
LFLAGS =

HDR    = crcmode.h
SRC    = crcmodel.c crctest.c 
OBJ    = crcmodel.o crctest.o
SOBJ   = source.o
SIOBJ  = sink.o
GOBJ   = gateway.o
EXE    = crctest
EXE2 = source gateway sink

all: $(EXE) $(EXE2)

%.o: %.c %.h
	$(CC) -c $(CLFAGS) $<

$(EXE): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(EXE)

source: $(SOBJ)
	$(CC) $(LFLAGS) $(SOBJ) -o source

sink: $(SIOBJ)
	$(CC) $(LFLAGS) $(SIOBJ) -o sink

gateway: $(GOBJ)
	$(CC) $(LFLAGS) $(GOBJ) -o gateway


clean:
	/bin/rm -f *.o *~ core $(EXE) $(EXE2)
