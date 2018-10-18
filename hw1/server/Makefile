.SUFFIXES : .c .o
CC = gcc

INC =
LIBS = 
CFLAGS = -g

OBJS = server.o
TARGET = server

all : 
	$(CC) $(CFLAGS) -o server *.c  $(LIBS)

clean :
	rm -rf $(TARGET) image.jpg core
new : 
	$(MAKE) clean 
	$(MAKE) 

