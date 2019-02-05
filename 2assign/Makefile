#
# CMSC257 - Utility Library 
# Code for the CMSC257 Projects
#

# Make environment
INCLUDES=-I.
CC=gcc
CFLAGS=-I. -c -g -Wall $(INCLUDES)
LINKARGS=-lm
LIBS=-lm

# Files
OBJECT_FILES=	assign2.o assign2-support.o

# Productions
all : assign2

assign2 : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

assign2.o : assign2.c assign2-support.h
	$(CC) $(CFLAGS) $< -o $@

assign2-support.o : a2support.c assign2-support.h 
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm -f assign2 $(OBJECT_FILES)
