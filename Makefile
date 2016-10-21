CC= gcc
CFLAGS = -std=c99 -pedantic -Wextra -Wall -Werror -Iinclude/
BIN_SRC= src/part1.c src/aux.c src/aux2.c
BIN_OBJS= $(BIN_SRC:.c=.o)

BIN=my_read_iso

all: $(BIN)

check: $(BIN)
	./tests/test.sh $(ex)

$(BIN): $(BIN_OBJS)
	$(CC) $^ -o $@

clean:
	${RM} ${BIN_OBJS}
	$(RM) $(BIN)

