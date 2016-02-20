CC = gcc
COMMOBJS = 
HEADERS  =list.h
SENDOBJS = list-sender.o list.o 
RECVOBJS = list-receiver.o list.o
LISTO = list.o
OBJS = $(SENDOBJS) $(RECVOBJS)$(COMMOBJS)$(LISTO)
CFLAGS = -Wall -g -std=gnu99
EXECS =	list-sender list-receiver

LFLAGS = 

all: $(EXECS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $<

list-sender: $(SENDOBJS)
	$(CC) -o $@ $(LFLAGS) $(SENDOBJS)

list-receiver: $(RECVOBJS)
	$(CC) -o $@ $(LFLAGS) $(RECVOBJS)


clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
