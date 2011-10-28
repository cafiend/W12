PROJECT = client

CC = clang
INCLUDE = ../../
HLIB = ../../hlib/hlib.a
CFLAGS = -Wall -g -I$(INCLUDE)

OBJECTS = client.o

all : $(PROJECT)

.c.o :
	$(CC) -c $(CFLAGS) $<

$(PROJECT) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROJECT) $(HLIB)


clean:
	rm -f *.o $(PROJECT)
