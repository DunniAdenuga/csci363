TCPLIB = .
CC     = gcc
LIB    = #-lsocket -lnsl
G      = -g
INCL   = tcplib.h http_response.h
SERVO  = webserver.o readln.o tcplib.o send_eof.o image_type.o
CLINO  = webclient.o readln.o tcplib.o send_eof.o
TESTO  = image_type.o test_image_type.o
EXE    = webserver webclient test_image_type

all: $(EXE)

webserver: $(SERVO)
	$(CC) -o webserver $(SERVO) $(LIB)
webclient: $(CLINO)
	$(CC) -o webclient $(CLINO) $(LIB)
test_image_type: $(TESTO)
	$(CC) -o test_image_type $(TESTO) $(LIB)

%.o: %.c $(INCL)
	$(CC) $G -c $<

clean:
	/bin/rm -f *.o core *~
realclean:
	/bin/rm -f *.o core *~ $(EXE)
