all:
	gcc -o assembler assembler.c

clean:
	rm assembler immediate.i 

install:
	sudo cp assembler /usr/bin/

count:
	wc definitions.h print.h symbol_literal_error.h assembler.c -l
