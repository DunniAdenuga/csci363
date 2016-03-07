CC   = gcc
H    = tcplib.h
LIB  = #-lnsl -lsocket
CFLG = -g -Wall
ALL  = superServer timeClient-udp echoClient getwho getwhoClient

all: $(ALL)

%.o: %.c $(H)
	$(CC) -c $(CFLG) $<

superServer: superServer.o tcplib.o
	$(CC) -o superServer superServer.o tcplib.o $(LIB)

timeClient-udp: timeClient-udp.o 
	$(CC) -o timeClient-udp timeClient-udp.o $(LIB)

echoClient: echoClient.o tcplib.o
	$(CC) -o echoClient echoClient.o tcplib.o $(LIB)

getwhoClient: getwhoClient.o tcplib.o
	$(CC) -o getwhoClient getwhoClient.o tcplib.o $(LIB)

getwho: getwho.o
	$(CC) -o getwho getwho.o

clean:
	/bin/rm -rf *.o core *~ $(ALL)
