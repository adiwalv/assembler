all:
	gcc -g -o assembler assembler.c

clean:
	rm assembler immediate.i *.o 

install:
	sudo cp assembler /usr/bin/

count:
	wc build/definitions.h build/print.h build/symbol_literal_error.h assembler.c -l
