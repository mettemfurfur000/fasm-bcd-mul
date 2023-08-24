#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "bcd_utils.c"

extern void add_bcd_btoe(byte *a, byte *b, byte *out, int size);
extern void add_bcd_etob(byte *a, byte *b, byte *out, int size);

void add_bcd_btoe_c(byte *pInput1, byte *pInput2, byte *pOutput, int size);

void add_bcd_etob_c(byte *pInput1, byte *pInput2, byte *pOutput, int size);

#define SIZE 10

int main()
{
	byte a[SIZE] = {0, 0, 0, 0, 0, 0, 0, 5, 1, 6};
	byte b[SIZE] = {0, 0, 0, 0, 0, 0, 0, 5, 2, 6};
	byte r[SIZE] = {0};
	byte r_c[SIZE] = {0};

	print_bcd(a, SIZE);
	print_bcd(b, SIZE);

	add_bcd_etob(a, b, r, SIZE);
	add_bcd_etob_c(a, b, r_c, SIZE);

	print_bcd(r, SIZE);
	print_bcd(r_c, SIZE);

	return 0;
}


void add_bcd_btoe_c(byte *pInput1, byte *pInput2, byte *pOutput, int size)
{
	byte carry_saver = 0;
	int i;

	for (i = 0; i < size; i++)
	{
		byte byte1 = *(pInput1++);
		byte byte2 = *(pInput2++);
		byte result = byte1 + byte2 + carry_saver;

		if ((result & 0x0f) > 0x09)
		{
			result += 0x06;
		}

		if ((result & 0xf0) > 0x90)
		{
			result += 0x60;
			carry_saver = 1;
		}
		else
		{
			carry_saver = 0;
		}

		*(pOutput++) = result;
	}
}

void add_bcd_etob_c(byte *pInput1, byte *pInput2, byte *pOutput, int size)
{
	byte carry_saver = 0;
	int i;

	pInput1 += size - 1;
	pInput2 += size - 1;
	pOutput += size - 1;

	for (i = 0; i < size; i++)
	{
		byte byte1 = *(pInput1--);
		byte byte2 = *(pInput2--);
		byte result = byte1 + byte2 + carry_saver;

		if ((result & 0x0f) > 0x09)
		{
			result += 0x06;
		}

		if ((result & 0xf0) > 0x90)
		{
			result += 0x60;
			carry_saver = 1;
		}
		else
		{
			carry_saver = 0;
		}

		*(pOutput--) = result;
	}
}
