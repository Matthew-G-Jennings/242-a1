#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/**
 * Provides helper functions to support hashtable functions.
 *
 * Error detecting memory allocation, prime number generation,
 * getting words and formatting them consistently.
 *
 * @author Jeremy Shanks, Will Johnstone, Matthew Jennings
 */

/**
 * Error-checking memory allocation. Attempts to allocate
 * memory. If memory allocation fails an error message will be printed to
 * stderr.
 *
 * @param s the size of the thing you wish to allocate memory for
 */
void *emalloc(size_t s){
    void *result = malloc(s);
    if (NULL == result){
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Error-checking memory reallocation. Attempts to reallocate
 * memory. If memory reallocation fails
 * an error message will be printed to stderr.
 *
 * @param p the pointer you wish to reallocate memory for
 * @param s the size of the thing you wish to reallocate memory for
 */
void *erealloc(void *p, size_t s){
    void *result = realloc(p, s);
    if (NULL == result){
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Manages the input of words from input STREAM.
 *
 * Example:
 *
 * while (getword(word, sizeof word, stdin) != EOF) {
 *     htable_insert(h, word);
 * }
 *
 * @param s string pointer to put the result in
 * @param limit max word size
 * @param stream specified stream for output
 *
 * @return w - s
 */
int getword(char *s, int limit, FILE *stream){
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    while(!isalnum(c = getc(stream)) && EOF != c)
    ;
    if (EOF == c){
        return EOF;
    } else if (--limit > 0){
        *w++ = tolower(c);
    }
    while(--limit > 0){
        if (isalnum(c = getc(stream))){
            *w++ = tolower(c);
        } else if ('\'' == c){
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/**
 * Tests to see whether or not the input integer is a prime number.
 * A prime number is a number that is divisible only by itself and one.
 *
 * @param n the input integer to test whether or not is prime.
 *
 * @return 1 if n is prime, 0 if n is not prime.
 */
int is_prime(int n){

    int i, flag;

    i = 2;
    flag = 0;

    for(i = 2; i < n / 2; i++){
        if(n % i == 0){
            flag = 1;
            break;
        }
    }

    if(flag == 0){
        return 1;
    }
    return 0;
}

/**
 * Find the next prime number after a given number.
 *
 * @param n the input integer to start looking for primes from.
 *
 * @return the next prime after the given input.
 */
int gen_prime(int n){
    while(is_prime(n++) == 0){}
    return n-1;
}