CC = gcc
COMMOBJS = 
HEADERS  = cookies.h readn.h writen.h
SERVOBJS = fortune-server.o cookies.o readn.o writen.o
COOKOBJS = fortune-client.o readn.o writen.o
COOKIESO = cookies.o
READO = readn.o
WRITEO =  writen.o
OBJS = $(SERVOBJS) $(COOKOBJS)$(COMMOBJS) $(COOKIESO) $(READO) $(WRITEO) 
CFLAGS = -Wall -g -std=gnu99
EXECS =	fortune-server  fortune-client 

LFLAGS = 

all: $(EXECS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $<

fortune-server: $(SERVOBJS)
	$(CC) -o $@ $(LFLAGS) $(SERVOBJS)

fortune-client: $(COOKOBJS)
	$(CC) -o $@ $(LFLAGS) $(COOKOBJS)


clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
