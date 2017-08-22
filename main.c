#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* NOTES: */
/* I'm using chars instead of ints to save space. Because I'm only storing digits in them, I have enough */
/* space to store two digits per char, but I don't want to mess with bit flipping */

/* This will probably only be fixed size (even for adding), but I might fix that later :) */

// TODO: This isn't standards compliant
// ifdef it into MSVC
// https://stackoverflow.com/a/3437484/2958070
#define MAX(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })

#define MIN(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _b : _a; })

struct bigint
{
    size_t digits_len;
    char* digits;
};
typedef struct bigint bigint;

/* Create a bigint from a char* and a size. */
/* produces a bigint of value 0 if digits is NULL */
/* digits should either be a char* of digits ( "11223249" ), or NULL */
/* digits_len should ALWAYS be >=  strlen(digits) if digits != NULL  or it will be truncated */
bigint* malloc_bigint(char* digits, size_t digits_len, size_t bigint_digits_len)
{
    // create the appropriate buffers and fill in what we can
    bigint* new_bigint = malloc(sizeof(bigint));
    new_bigint->digits_len = bigint_digits_len;

    char* new_buffer = malloc(sizeof(char) * bigint_digits_len);
    memset(new_buffer, 0, bigint_digits_len);

    if (digits)
    {
        // copy the digits into buffer converting from chars to digits ones place first!
        size_t i = 0;
        for (size_t j = digits_len - 1; j > 0; --j)
        {
            new_buffer[i] = digits[j] - '0';
            ++i;
        }
        new_buffer[i] = digits[0] - '0';
    }
    new_bigint->digits = new_buffer;
    return new_bigint;
}

void free_bigint(bigint* bi)
{
    free(bi->digits);
    free(bi);
}

void printf_bigint(bigint* bi)
{
    printf("size: %zu\n", bi->digits_len);
    printf("digits: ");
    for (size_t i = bi->digits_len - 1; i > 0; --i)
    {
        printf("%c", bi->digits[i] + '0');
    }
    printf("%c", bi->digits[0] + '0');
    printf("\n");
}

bigint* add(bigint* bi1, bigint* bi2)
{
    // Allocate one more digit for the answer
    // because that's the max it can reach
    // TODO: better allocation strategy?
    size_t new_len = MAX(bi1->digits_len, bi2->digits_len) + 1;
    bigint* ans = malloc_bigint(NULL, 0, new_len);
    // copy b1 into ans
    memcpy(ans->digits, bi1->digits, bi1->digits_len);

    char carry = 0;
    char next_carry = 0;
    for(size_t i = 0; i < bi2->digits_len; ++i)
    {
        char first_digit = ans->digits[i];
        char second_digit = bi2->digits[i];
        char digit_sum = first_digit + second_digit + carry;
        // it's expected to be less than 18
        // TODO: fancy branchless bit expression
        if (digit_sum >= 10) {
            next_carry = 1;
            digit_sum = digit_sum - 10;
        }
        else
        {
            next_carry = 0;
        }
        /* printf("%c + %c + %c = %c\n", */
        /*     first_digit + '0', */
        /*     second_digit + '0', */
        /*     carry + '0', */
        /*     digit_sum + '0'); */
        carry = next_carry;
        ans->digits[i] = digit_sum;
    }

    // handle the carry bit on the last digit
    /* ans->digits[bi2->digits_len] += carry; */
    if (carry)
    {
        size_t i = bi2->digits_len;
        for(; ans->digits[i] == 9; ++i)
        {
            ans->digits[i] = 0;
        }
        ans->digits[i] += carry;
    }
    return ans;
}

void test_malloc(char* digits, size_t digits_len, size_t bigint_digits_len)
{
    printf("bigint* a = malloc_bigint(\"%s\", %zu, %zu);\n", digits, digits_len, bigint_digits_len);
    bigint* a = malloc_bigint(digits, digits_len, bigint_digits_len);
    printf_bigint(a);
    free_bigint(a);
    printf("\n");
}

void test_add(char* a, char* b)
{
    printf("Adding: %s + %s\n", a, b);
    bigint* bia = malloc_bigint(a, strlen(a), strlen(a));
    bigint* bib = malloc_bigint(b, strlen(b), strlen(b));
    bigint* bis = add(bia, bib);
    printf_bigint(bis);
    free_bigint(bis);
    free_bigint(bia);
    free_bigint(bib);
}


void test()
{
#if 1
    test_malloc("12345", 5, 4);
    test_malloc("12345", 5, 6);
    test_malloc(NULL, 2, 3);
#endif
#if 1
    test_add("9875", "234");
    test_add("99999999999999999999999875", "234");
#endif
}

int main(int argc, char** argv)
{
    (void) argc;
    bigint* bia = malloc_bigint(argv[1], strlen(argv[1]), strlen(argv[1]));
    bigint* bib = malloc_bigint(argv[2], strlen(argv[2]), strlen(argv[2]));
    bigint* bis = add(bia, bib);

    // get to the non-zero digits
    size_t i = bis->digits_len -1;
    while (bis->digits[i] == 0)
    {
        --i;
    }

    // now start printing
    while (i > 0)
    {
        printf("%c", bis->digits[i] + '0');
        --i;
    }
    printf("%c\n", bis->digits[0] + '0');

    free_bigint(bis);
    free_bigint(bia);
    free_bigint(bib);
}
