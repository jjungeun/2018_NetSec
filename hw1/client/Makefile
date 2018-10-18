.SUFFIXES : .c .o
CC = gcc

INC =
LIBS = 
CFLAGS = -g

OBJS = client.o
TARGET = client

all : 
	$(CC) $(CFLAGS) -o client *.c  $(LIBS)

clean :
	rm -rf $(TARGET) core
new : 
	$(MAKE) clean 
	$(MAKE) 

