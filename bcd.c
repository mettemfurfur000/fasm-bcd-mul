#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char byte;

extern void add_bcd_btoe(byte *a, byte *b, byte *out, int size);
extern void add_bcd_etob(byte *a, byte *b, byte *out, int size);
extern void bcd_mul(byte *a, byte *b, byte *out, int size);

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

void bcd_multiply(byte *num1, byte *num2, byte *result, int size)
{
    // Initialize result array with zeros
    for (int i = 0; i < 2 * size; i++)
    {
        result[i] = 0;
    }

    // Multiply each digit of num1 with num2 and add to result
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Multiply digits and adjust for BCD
            byte digit_product = (num1[i] & 0x0f) * (num2[j] & 0x0f);
            byte tens_product = (num1[i] >> 4) * (num2[j] >> 4);
            byte carry = 0;

            // Add digit product to result
            int digit_index = i + j;
            byte digit_sum = result[digit_index] + (digit_product & 0x0f);
            if (digit_sum > 0x09)
            {
                digit_sum += 0x06;
            }
            result[digit_index] = digit_sum & 0x0f;

            // Add tens product to result
            int tens_index = i + j + 1;
            byte tens_sum = result[tens_index] + (digit_sum >> 4) + (tens_product & 0x0f);
            if (tens_sum > 0x09)
            {
                tens_sum += 0x06;
            }
            if (tens_sum > 0x99)
            {
                carry = 1;
                tens_sum -= 0xa0;
            }
            result[tens_index] = tens_sum & 0x0f;

            // Propagate carry to higher digits
            for (int k = tens_index + 1; k < 2 * size; k++)
            {
                byte sum = result[k] + carry;
                if (sum > 0x09)
                {
                    sum += 0x06;
                }
                if (sum > 0x99)
                {
                    carry = 1;
                    sum -= 0xa0;
                }
                else
                {
                    carry = 0;
                }
                result[k] = sum & 0x0f;
            }
        }
    }
}

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

clock_t bench_bcd_impl(void (*bcd_fun)(byte *, byte *, byte *, int), int size, int runs)
{
    byte *a = (byte *)malloc(size);
    byte *b = (byte *)malloc(size);
    byte *r = (byte *)malloc(size);

    clock_t start = clock();

    for (int i = 0; i < runs; i++)
    {
        fill_test_data(a, size);
        fill_test_data(b, size);
        bcd_fun(a, b, r, size);
    }

    clock_t time_total = clock() - start;

    free(a);
    free(b);
    free(r);

    return time_total;
}

// void bench_both_impl()
// {
//     for (int i = 1; i < 10; i++)
//     {
//         int size = 100 * (i * i * i);
//         int runs = 100 * (i * i * i);
//         clock_t fasm_t = bench_bcd_impl(&add_bcd_btoe, size, runs);
//         clock_t c_t = bench_bcd_impl(&add_bcd_btoe_c, size, runs);

//         printf("bench size:%d\truns:%d\tfasm:%.4fs\tc:%.4fs\n", size, runs, (double)fasm_t / CLOCKS_PER_SEC, (double)c_t / CLOCKS_PER_SEC);
//     }
// }

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

void wrapper_bcd_mul(byte *a, byte *b, byte *r, int size)
{
    print_bcd(a, size);
    print_bcd(b, size);

    bcd_mul(a, b, r, size);

    print_bcd(r, size * 2);
}

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