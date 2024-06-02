CROSS_COMPILE = gcc
CFLAGS=-g -Wall 


all: aesdsocket
default: aesdsocket

aesdsocket: aesdsocket.c
	$(CROSS_COMPILE) $(CFLAGS) aesdsocket.c -o aesdsocket

clean:
	rm -rf aesdsocket *.o