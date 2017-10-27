CC = gcc
IDIRS = util 
CFLAGS = -I$(IDIRS)
LDLIBS = -lm 
PROG = main.exe
OBJS = main.o bitstring.o classifier.o pattern.o list.o random.o set.o
VPATH = util/bits:util/collections:util/core

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LDLIBS) $(CFLAGS) $(OBJS) -o $@ 


main.o: classifier.h
classifier.o: bitstring.h pattern.h
pattern.o: bitstring.h random.h
bitstring.o: bitstring.h random.h

list.o: list.h
set.o: set.h list.h
random.o: random.h





