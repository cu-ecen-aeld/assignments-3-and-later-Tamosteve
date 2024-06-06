# Compiler options
CC = $(CROSS_COMPILE)gcc
# CROSS_COMPILE = aarch64-none-linux-gnu
CFLAGS = -Wall -Wextra -Werror

all: aesdsocket

aesdsocket: aesdsocket.o
	$(CC) $(CFLAGS) -o aesdsocket aesdsocket.o

aesdsocket.o: aesdsocket.c
	$(CC) $(CFLAGS) -c aesdsocket.c

clean:
	rm -f aesdsocket *.o