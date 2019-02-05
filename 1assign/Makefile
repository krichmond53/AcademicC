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
OBJECT_FILES=	cmsc257-f16-assign1.o \
				cmsc257-f16-assign1-support.o

# Productions
all : cmsc257-f16-assign1

cmsc257-f16-assign1 : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

cmsc257-f16-assign1.o : cmsc257-f16-assign1.c cmsc257-f16-assign1-support.h
	$(CC) $(CFLAGS) $< -o $@

cmsc257-f16-assign1-support.o : a1support.c cmsc257-f16-assign1-support.h 
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm -f cmsc257-f16-assign1 $(OBJECT_FILES)
