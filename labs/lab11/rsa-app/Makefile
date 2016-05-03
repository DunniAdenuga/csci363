CC      = gcc
CFLAGS  = -Wall -g 
SLIB    = -lssl #-lnsl -lsocket -lcurses
CLIB    = -lssl #-lcurses
H       = rsa.h read-writen.h tcplib-ssl.h
COMOBJ  = tcplib-ssl.o rsa.o read-writen.o
COMSRC  = tcplib-ssl.c rsa.c read-writen.c
SRVSRC  = echo-server.c $(COMSRC)
SRVOBJ  = echo-server.o $(COMOBJ)
CLNSRC  = echo-client.c $(COMSRC)
CLNOBJ  = echo-client.o $(COMOBJ)
EXE     = echo-client echo-server

all: $(EXE) 

%.o: %.c $(H)
	$(CC) -c $(CFLAGS) $<

echo-client: $(CLNOBJ)
	$(CC) -o $@ $(CLNOBJ) $(CLIB)

echo-server: $(SRVOBJ)
	$(CC) -o $@ $(SRVOBJ) $(SLIB)

clean:
	/bin/rm -rf *~ *.o core $(EXE)
