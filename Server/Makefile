CC = gcc

CFLAGS = -I../Shared

SRC = chatroom.c client.c main.c server.c ../Shared/protocol.c ../Shared/util.c

OBJ = $(SRC:.c=.o)

#LDFLAGS = ../Shared/protocol.c

TARGET = server

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)