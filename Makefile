all:
	gcc -o assembler assembler.c

clean:
	rm assembler immediate.i
