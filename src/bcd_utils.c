#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char byte;

int print_bcd(byte *a, int size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        printf("%x ", a[i]);
    }
    printf("\n");
    return 0;
}

void fill_test_data(byte *a, int size)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 0x0a + ((rand() % 0x0a) << 4);
    }
}

void flip_array(byte *a, int size)
{
    if (!size)
        return;

    for (int i = 0; i < size / 2; i++)
    {
        int temp = a[i];
        a[i] = a[size - i - 1];
        a[size - i - 1] = temp;
    }
}