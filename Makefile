CC= gcc
CFLAGS = -std=c99 -pedantic -Wextra -Wall -g -Iinclude/
BIN_SRC= src/part1.c
BIN_OBJS= $(BIN_SRC:.c=.o)

BIN=my_read_iso

all: $(BIN)

check: $(BIN)
	./tests/bin.sh


$(BIN): $(BIN_OBJS)
	$(CC) $^ -o $@

clean:
	${RM} ${BIN_OBJS}
	$(RM) $(BIN)

