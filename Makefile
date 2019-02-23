SRC = server.c
OBJ = server

CC = gcc
CFLAGS = -g -o

LIB = -lpthread
RM  = rm -f 

all:
	$(CC) $(CFLAGS) $(OBJ) $(SRC) $(LIB)

clean:
	$(RM) $(OBJ)
