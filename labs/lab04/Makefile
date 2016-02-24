CC = gcc
COMMOBJS = 
HEADERS  =list.h
SENDOBJS = list-sender.o list.o 
RECVOBJS = list-receiver.o list.o
LISTO = list.o
OBJS = $(SENDOBJS) $(RECVOBJS)$(COMMOBJS)$(LISTO)inet-checksum-skeleton.o test-checksum.o test-sizeof.o udp-sender.o reflectord.o
CFLAGS = -Wall -g -std=gnu99
EXECS =	list-sender list-receiver test-checksum test-sizeof udp-sender reflectord

LFLAGS = 

all: $(EXECS)

%.o: %.c 
	$(CC) -c $(CFLAGS) $<

list-sender: $(SENDOBJS)
	$(CC) -o $@ $(LFLAGS) $(SENDOBJS)

list-receiver: $(RECVOBJS)
	$(CC) -o $@ $(LFLAGS) $(RECVOBJS)

test-checksum: inet-checksum-skeleton.o test-checksum.o
	$(CC) -o $@ $(LFLAGS) inet-checksum-skeleton.o test-checksum.o

udp-sender:  udp-sender.o inet-checksum-skeleton.o
	$(CC) -o $@ $(LFLAGS) udp-sender.o inet-checksum-skeleton.o

reflectord:  reflectord.o inet-checksum-skeleton.o
	$(CC) -o $@ $(LFLAGS) reflectord.o inet-checksum-skeleton.o

test-sizeof: test-sizeof.o
	$(CC) -o $@ $(LFLAGS) test-sizeof.o

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
