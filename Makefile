CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin/lunara

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(OBJ)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
