CROSS_COMPILE = aarch64-none-linux-gnu
CFLAGS = -Wall -Wextra -Werror
CC = gcc


all: aesdsocket CROSS_COMPILE

aesdsocket: aesdsocket.c
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket

CROSS_COMPILE: aesdsocket.c
	$(CROSS_COMPILE)-$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket-c

clean:
	rm -rf aesdsocket *.o