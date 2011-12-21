all: os1shell

CC=gcc
CFLAGS=-c -Wall -g

os1shell: os1shell.o exec_cmd.o hist.o
	$(CC) os1shell.o exec_cmd.o hist.o -o os1shell

os1shell.o: os1shell.c
	$(CC) $(CFLAGS) os1shell.c

exec_cmd.o: exec_cmd.c
	$(CC) $(CFLAGS) exec_cmd.c

hist.o: hist.c
	$(CC) $(CFLAGS) hist.c

clean:
	rm -rf *.o

