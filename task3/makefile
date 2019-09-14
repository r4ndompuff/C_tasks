CC = gcc

all: start.out
	echo "Build Done!"

start.out: main.o list.o
	$(CC) main.o list.o

main.o: main.c defs.h
	$(CC) -c -Wall main.c -g

list.o: list.o defs.h
	$(CC) -c -Wall list.c -g

clean:
	rm -f *.out *.o

# In cmd: $ make all -f makefile.make
