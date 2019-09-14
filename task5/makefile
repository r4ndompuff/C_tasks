C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS :=
CC_FLAGS := -Wall -Wextra -g

shell: $(OBJ_FILES)
	gcc $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.c
	gcc $(CC_FLAGS) -c -o $@ $<

clean:
	rm -rf shell ./obj/*.o

run:
	./shell

valgrind:
	valgrind --track-origins=yes --leak-check=full ./shell

help:
	echo "You can use make with shell/clean/run/valgrind/help"