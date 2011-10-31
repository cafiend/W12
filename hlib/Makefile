PROJECT = hlib.a

CC = gcc 
CFLAGS = -Wall -g 


OBJECTS = socket.o command.o event.o callback.o hlib.o 

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	ar rcs $(PROJECT) $(OBJECTS)

socket.o: socket.h
command.o: command.h
event.o: event.h
callback.o : callback.h
hlib.o: socket.h command.h event.h callback.h hlib.h 

.PHONY : clean
clean :
	rm $(PROJECT) $(OBJECTS)

