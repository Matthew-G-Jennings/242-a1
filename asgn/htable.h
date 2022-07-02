#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

/**
 * Defines a hashtable and it's operations.
 *
 * Default collision resolution is linear probing, changed by
 * passing a value of DOUBLE_H as hashing_t when initializing the
 * table.
 *
 * @author Jeremy Shanks, Will Johnstone, Matthew Jennings
 */

/* Defines a htable stucture */
typedef struct htablerec *htable;

/* Defines an enumerated type to determine collision resolution*/
typedef enum hashing_e { LINEAR_P, DOUBLE_H } hashing_t;

/**
 * Frees all memory that has been malloced associated with
 * the given hashtable.
 *
 * @param h hashtable to free
 */
extern void htable_free(htable h);

/**
 * Creates a new hashtable and allocates memory as required.
 *
 * @param ht what kind of collision resolution strategy
 * @param int the intended capacity of the new hashtable
 *
 * @return the newly created hash table
 */
extern htable htable_new(hashing_t ht, int capacity);

/**
 * Insertion function. Inserts a given string into a given htable.
 * Allocates memory as required.
 *
 * @param h hashtable to insert in to
 * @param str the string to insert
 *
 * @return a positive int of the number of occurences of the input after
 * insertion if successful, 0 if not successful.
 */
extern int htable_insert(htable h, char *str);

/**
 * Prints the entire hash table in a debugging style format.
 *
 * @param h input hashtable to print
 * @param stream the output stream to print to
 */
extern void htable_print(htable h, FILE *stream);

/**
 * Searches a given hashtable for a given word.
 *
 * @param h hashtable to search
 * @param str string variable to search
 *
 * @return the frequency the word exists with or 0 if it is
 * not present in the table.
 */
extern int htable_search(htable h, char *str);

/**
 * Prints the entire hash table in a tidy format.
 *
 * @param h input hashtable to print
 * @param stream output stream to print to
 */
extern void htable_print_entire_table(htable h, FILE *stream);

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
extern void htable_print_stats(htable h, FILE *stream, int num_stats);

#endif
