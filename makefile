mul:
	fasm src/bcd_mul.asm obj/bcd_mul_asm.o
	gcc -m32 -no-pie -g -O0 src/bcd_mul.c obj/bcd_mul_asm.o -o bin/bcd_mul

add:
	fasm src/bcd_add.asm obj/bcd_add_asm.o
	gcc -m32 -no-pie -g -O0 src/bcd_add.c obj/bcd_add_asm.o -o bin/bcd_add

all: mul add
