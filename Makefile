all:
	gcc -g -o assembler assembler.c
	gcc -g -o smaco smaco.c

clean:
	rm assembler smaco immediate.i *.o *.lst 

install:
	sudo cp assembler /usr/bin/

count:
	wc build/definitions.h build/print.h build/symbol_literal_error.h assembler.c smaco.c build/generate_object.h -l
