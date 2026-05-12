#!/usr/bin/sh

rm -rf ~/.cache/Prayer/

CC=(cc)
CFLAGS=(-g)
LIBS=(-lcurl -lm -ljson-c)
SRC=src/*.c

$CC "${CFLAGS[@]}" "${LIBS[@]}" -D DEBUG -o main $SRC &&
    valgrind -v --track-fds=yes --leak-check=full --show-leak-kinds=all ./main
