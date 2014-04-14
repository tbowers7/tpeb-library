CFLAGS += -Wall -L/sw/lib -I/sw/include
CC = gcc-4

SOURCES = $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)


object: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

lib: $(OBJECTS)
	ar -rcs libtpeb.a $(OBJECTS)

clean:
	rm -f *.o
	rm -f libtpeb.a

