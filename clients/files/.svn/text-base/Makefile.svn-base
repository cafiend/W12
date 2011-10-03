PROJECT = client

CC = clang
INCLUDE = ../../
HLIB = ../../hlib/hlib.a
CFLAGS = -Wall -std=c99 -pedantic -g -I$(INCLUDE)

OBJECTS = icon.o fileview.o client.o

all : $(PROJECT)

icon.o: icon.h
fileview.o: icon.h
client.o: icon.h fileview.h

$(PROJECT) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROJECT) $(HLIB)


clean:
	rm -f *.o $(PROJECT)


