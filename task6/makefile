phil: phil.o
	gcc -pthread phil.c -o phil
	echo "\033[37;1;41m Type: make run \033[0m"

phil2: phil2.o
	gcc -pthread phil2.c -o phil2
	echo "\033[30;30;43m Type: make run2 \033[0m"

clean:
	rm -rf phil phil2 *.o

run:
	./phil

run2:
	./phil2

valgrind:
	valgrind --track-origins=yes --leak-check=full ./phil

help:
	echo "Make phil (run) - for systems with normal UNIX-kernel, make phil2 (run2) - for bad system UNIX-kernel"