CFLAGS = -g -Wall -pedantic
CC = gcc

all: mytree

mytree: cmdline.o mytree.o mytree.h
	$(CC) $(CFLAGS) $^ -o $@

%: %.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o a.out
	rm -f cmdline
	rm -f mytree
