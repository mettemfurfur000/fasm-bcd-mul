all:
	clear
	fasm bcd.asm bcdasm.o
	gcc -m32 -no-pie -g -O0 bcd.c bcdasm.o -o bcd
	./bcd