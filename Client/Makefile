CC = gcc

CFLAGS = -I../Shared

LDFLAGS = ../Shared/protocol.c

SRC = chatroom_client.c main.c

OBJ = $(SRC:.c=.o)

TARGET = client

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)