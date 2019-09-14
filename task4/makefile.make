all: cmp cp echo ls pwd tail

%.o: %.c    #Suppress default rule
%: %.c
	gcc -o $@ $<