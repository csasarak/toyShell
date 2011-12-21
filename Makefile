all: os1shell

CC=gcc
CFLAGS=-c -Wall

os1shell: os1shell.o cmd_exec.o hist.o
	$(CC) os1shell.o cmd_exec.o hist.o -o os1shell

os1shell.o: os1shell.c
	$(CC) $(CFLAGS) os1shell.c

cmd_exec.o: cmd_exec.c
	$(CC) $(CFLAGS) cmd_exec.c

hist.o: hist.c
	$(CC) $(CFLAGS) hist.c

