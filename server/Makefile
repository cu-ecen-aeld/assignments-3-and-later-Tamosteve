# Compiler options
CC = gcc
CROSS_COMPILE = aarch64-none-linux-gnu
CFLAGS = -Wall -Wextra -Werror

all: aesdsocket

aesdsocket: aesdsocket.c
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket

CROSS_COMPILE: aesdsocket.c
	$(CROSS_COMPILE)-$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket-c

clean:
	rm -rf aesdsocket aesdsocket-c *.o