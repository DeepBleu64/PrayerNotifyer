CC = gcc
CFLAGS = -Wall -Wextra
SRC = src/*.c
LIBS = -lcurl -lm -ljson-c
DIR  = /usr/local/bin
NAME = prayer-notifyer

install:
	$(CC) $(CFLAGS) $(LIBS) $(SRC) -o $(DIR)/$(NAME)

# for debugging
install-verbose:

	 $(CC) $(CFLAGS) $(LIBS) -D DEBUG $(SRC) -g -fsanitize=address -o main


uninstall:

	rm -i $(DIR)/$(NAME)
