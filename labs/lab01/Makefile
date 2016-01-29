CC   = gcc
G    = -g
H    = dlist.h
SRC  = dlist.c test_dlist.c
OBJ  = dlist.o test_dlist.o
EXE  = test_dlist

all: $(EXE)

%.o : %.c $(H)
	$(CC) $G -c $<

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ)

clean:
	/bin/rm -f $(OBJ) *~ a.out
realclean:
	/bin/rm -f $(OBJ) *~ a.out $(EXE)
