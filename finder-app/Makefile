# Compiler options
CC = gcc
CROSS_COMPILE = aarch64-none-linux-gnu
CFLAGS = -Wall -Wextra
LDFLAGS=
all: CROSS_COMPILE

writer: writer.c
	$(CC) writer.c -o writer

CROSS_COMPILE: writer.c
	$(CROSS_COMPILE)-$(CC) writer.c -o writer-cc

clean:
	rm  -rf writer writer-cc *.o


