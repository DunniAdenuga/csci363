CC   = gcc
NLIB = -lcrypt -lreadline #-lnsl -lsocket -lcurses
CLIB = -lcrypt #-lcurses
EXE  = rshd  mlogin mypasswd rshClient
CFLAGS    = -Wall -g

all: $(EXE) 

%.o: %.c
	$(CC) -c $(CFLAGS) $<

rshd: rsh.o rshd.o tcplib.o 
	$(CC) -o rshd rsh.o rshd.o tcplib.o $(CLIB)

rshClient: rshClient.o tcplib.o
	$(CC) -o rshClient rshClient.o tcplib.o $(NLIB)

mlogin: login.o shadow.o pwd.o
	$(CC) -o mlogin login.o shadow.o pwd.o $(CLIB)

mypasswd: mypasswd.o shadow.o pwd.o
	$(CC) -o mypasswd mypasswd.o shadow.o pwd.o $(CLIB)
clean:
	/bin/rm -rf *~ *.o core $(EXE)
