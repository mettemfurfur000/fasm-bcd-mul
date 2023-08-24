#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "bcd_utils.c"

extern void bcd_mul(byte *a, byte *b, byte *out, int size);

#define SIZE 10

int main()
{
    byte a[SIZE] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 1}; 
    byte b[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0}; 
    byte r[SIZE * 2] = {0};

    print_bcd(a, SIZE); // 1123456789
    print_bcd(b, SIZE); // 0987654321

    bcd_mul(a, b, r, SIZE); // 1123456789 * 0987654321

    print_bcd(r, SIZE * 2); // result is 01109586952112635269

    return 0;
}