CC = gcc
COMMOBJS = 
HEADERS  = cookies.h readn.h writen.h
SERVOBJS = fortune-server.o cookies.o readn.o writen.o
COOKOBJS = fortune-client.o readn.o writen.o
STATOBJS = stats-client.o readn.o writen.o
COOKIESO = cookies.o
SERVDOBJS = fortune-serverd.o cookies.o readn.o writen.o
READO = readn.o
WRITEO =  writen.o
OBJS = $(SERVOBJS) $(COOKOBJS) $(STATOBJS) $(COMMOBJS) $(COOKIESO) $(READO) $(WRITEO) $(SERVDOBJS)
CFLAGS = -Wall -g -std=gnu99
EXECS =	fortune-server fortune-serverd fortune-client stats-client

LFLAGS = 

all: $(EXECS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $<

fortune-server: $(SERVOBJS)
	$(CC) -o $@ $(LFLAGS) $(SERVOBJS)

fortune-serverd: $(SERVDOBJS)
	$(CC) -o $@ $(LFLAGS) $(SERVDOBJS)

fortune-client: $(COOKOBJS)
	$(CC) -o $@ $(LFLAGS) $(COOKOBJS)

stats-client: $(STATOBJS)
	$(CC) -o $@ $(LFLAGS) $(STATOBJS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
