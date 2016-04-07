# Makefile for CSCI 363 Computer Networks, Lab 7
# Author: L. Felipe Perrone
# Date: 2012-28-02
#
# Copyright (c) 2012 Bucknell University
#
# Permission is hereby granted, free of charge, to any individual or
# institution obtaining a copy of this software and associated
# documentation files (the "Software"), to use, copy, modify, and
# distribute without restriction, provided that this copyright and
# permission notice is maintained, intact, in all copies and supporting
# documentation.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL BUCKNELL UNIVERSITY BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# Modified by Xiannong Meng
# Date: 2013-03-03
# Removed dependency on libpcap

CC = gcc

HDRS = prot_strings.h etherTrace.h
SRCS = prot_strings.c etherTrace.c hexdump.c
OBJS = prot_strings.o etherTrace.o hexdump.o
EXECS =	etherTrace

CFLAGS = -g -Wall
LFLAGS = #-lpcap

all: $(EXECS)

%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@

etherTrace: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LFLAGS)

clean:
	/bin/rm -f $(OBJS) $(EXECS) core* *~ semantic.cache
