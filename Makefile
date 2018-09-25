all:
	gcc -o assembler assembler.c

clean:
	rm assembler immediate.i 

install:
	sudo cp assembler /usr/bin/
