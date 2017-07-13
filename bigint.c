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
        // copy the digits into buffer converting from chars to digits
        // TODO: I should probably use memcopy (it's probably vectorized) and subtract '0' in a separate loop
        char* current_digits_char = digits + digits_len - 1;
        char* current_new_buffer_char = new_buffer + bigint_digits_len - 1;
        while((current_digits_char - digits) && (current_new_buffer_char - new_buffer))
        {
            *(current_new_buffer_char) = *(current_digits_char) - '0';
            current_digits_char--;
            current_new_buffer_char--;
        }
        *(current_new_buffer_char) = *(current_digits_char) - '0';
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
    for(size_t i = 0; i < bi->digits_len; ++i)
    {
        printf("%c", bi->digits[i] + '0');
    }
    printf("\n");
}

bigint* add(bigint* bi1, bigint* bi2)
{
    // Allocate one more digit for the answer
    // because that's the max it can reach
    // TODO: better allocation strategy?
    size_t new_len = MAX(bi1->digits_len, bi2->digits_len) + 1;
    bigint* ans = malloc_bigint(NULL, 0, new_len);
    // TODO: better stopping strategy...I can stop when both are 0
    // copy b1 into ans
    memcpy(ans->digits + ans->digits_len - bi1->digits_len,
        bi1->digits, bi1->digits_len);
    // add b2 to ans
    char* current_ans_digit = ans->digits + ans->digits_len - 1;
    char* current bi2_digit = bi2->digits + bi2->digits_len - 1;

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
    bigint* bia = malloc_bigint(a, strlen(a), strlen(a));
    bigint* bib = malloc_bigint(a, strlen(b), strlen(b));
    printf_bigint(add(bia, bib));
    free_bigint(bia);
    free_bigint(bib);
}


int main()
{
#if 0
    test_malloc("12345", 5, 4);
    test_malloc("12345", 5, 6);
    test_malloc(NULL, 2, 3);
#endif
#if 1
    test_add("12221", "234");
#endif
}
