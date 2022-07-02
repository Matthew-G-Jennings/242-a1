#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

/**
 * Defines a hashtable and it's operations.
 *
 * Default collision resolution is linear probing, changed by
 * passing a value of DOUBLE_H as hashing_t when initializing the
 * table.
 *
 * @author Jeremy Shanks, Will Johnstone, Matthew Jennings
 */

struct htablerec{
    int capacity;
    int num_keys;
    char **keys;
    int *freqs;
    int *stats;
    hashing_t method;
};

/**
 * Generates an integer representation of a given string.
 *
 * @param word the string to be converted to an unsigned integer
 *
 * @return an unsigned integer that is the sum of all characters in word
 */
static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;
    while (*word != '\0'){
        result = (*word++ + 31 * result);
    }
    return result;
}

/**
 * Step calculation function. Only used for double hashing.
 *
 * @param h hashtable to insert in to
 * @param i_key the index value at the stage of stepping
 *
 * @return an unsigned int representing the step
 */
static unsigned int htable_step(htable h, unsigned int i_key){
    return 1 + (i_key % (h->capacity - 1));
}

/**
 * Frees all memory that has been malloced associated with
 * the given hashtable.
 *
 * @param h hashtable to free
 */
void htable_free(htable h){
    int i;

    for (i = 0; i < h->capacity; i++){
        if (h->freqs[i] != 0){
            free(h->keys[i]);
        }
    }

    free(h->stats);
    free(h->freqs);
    free(h->keys);
    free(h);
}

/**
 * Creates a new hashtable and allocates memory as required.
 *
 * @param ht what kind of collision resolution strategy
 * @param int the intended capacity of the new hashtable
 *
 * @return the newly created hash table
 */
htable htable_new(hashing_t ht, int capacity){

    int i;
    htable result;

    result = emalloc(sizeof *result);

    if(ht == LINEAR_P){
        result->method = LINEAR_P;
    } else if (ht == DOUBLE_H){
        result->method = DOUBLE_H;
    }

    result->capacity = capacity;
    result->num_keys = 0;
    result->keys = emalloc(result->capacity * sizeof result->keys[0]);
    result->freqs = emalloc(result->capacity * sizeof result->freqs[0]);
    result->stats = emalloc(result->capacity * sizeof result->stats[0]);

    for (i = 0; i < result->capacity; i++){
        result->stats[i] = 0;
        result->freqs[i] = 0;
        result->keys[i] = NULL;
    }
    return result;
}

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
int htable_insert(htable h, char *str){

    int collisions, step, index;
    unsigned int i_key;

    i_key = htable_word_to_int(str);
    if (h->method == DOUBLE_H){
        step = htable_step(h, i_key);
    } else {
        step = 1;
    }

    index = i_key % h->capacity;
    collisions = 0;

    while (collisions < h->capacity){
        if (h->freqs[index] == 0){
            h->keys[index] = emalloc((strlen(str) + 1) * sizeof h->keys[0][0]);
            strcpy(h->keys[index], str);
            h->freqs[index] = (h->freqs[index] + 1);
            h->stats[h->num_keys] = collisions;
            h->num_keys++;
            return 1;

        } else if (strcmp(h->keys[index], str) == 0){
            h->freqs[index] = (h->freqs[index] + 1);
            return h->freqs[index];

        } else {
            collisions++;
            index = (index + step) % h->capacity;
            continue;
        }
    }
    return 0;
}

/**
 * Prints the entire hash table in a debugging style format.
 *
 * @param h input hashtable to print
 * @param stream the output stream to print to
 */
void htable_print(htable h, FILE *stream){
    int i;
    for (i = 0; i < h->capacity; i++) {
        if (h->freqs[i] > 0){
            fprintf(stream, "%-4d %s\n", h->freqs[i], h->keys[i]);
        }
    }
}

/**
 * Searches a given hashtable for a given word.
 *
 * @param h hashtable to search
 * @param str string variable to search
 *
 * @return the frequency the word exists with or 0 if it is
 * not present in the table.
 */
int htable_search(htable h, char *str){
    int collisions, step, index;
    unsigned int i_key;

    i_key = htable_word_to_int(str);

    if (h->method == DOUBLE_H){
        step = htable_step(h, i_key);
    } else {
        step = 1;
    }

    collisions = 0;

    index = i_key % h->capacity;

    while (h->freqs[index] != 0 && collisions<h->capacity){
        if(strcmp(h->keys[index], str)==0){
            break;
        } else {
            collisions++;
            index = (index + step) % h->capacity;
        }
    }
    if (collisions >= h->capacity){
        return 0;
    } else {
        return h->freqs[index];
    }
}

/**
 * Prints the entire hash table in a tidy format.
 *
 * @param h input hashtable to print
 * @param stream output stream to print to
 */
void htable_print_entire_table(htable h, FILE *stream){

    int i;

    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream, "----------------------------------------\n");

    for(i = 0; i < h->capacity; i++){

        if (h->keys[i] != NULL){
            fprintf(stream, "%5d %5d %5d   %s\n", i,
                    h->freqs[i], h->stats[i], h->keys[i]);
        } else{
            fprintf(stream, "%5d %5d %5d\n", i, h->freqs[i], h->stats[i]);
        }
    }
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
   int current_entries = h->capacity * percent_full / 100;
   double average_collisions = 0.0;
   int at_home = 0;
   int max_collisions = 0;
   int i = 0;

   if (current_entries > 0 && current_entries <= h->num_keys) {
      for (i = 0; i < current_entries; i++) {
         if (h->stats[i] == 0) {
            at_home++;
         }
         if (h->stats[i] > max_collisions) {
            max_collisions = h->stats[i];
         }
         average_collisions += h->stats[i];
      }

      fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full,
              current_entries, at_home * 100.0 / current_entries,
              average_collisions / current_entries, max_collisions);
   }
}

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
void htable_print_stats(htable h, FILE *stream, int num_stats) {
   int i;

   fprintf(stream, "\n%s\n\n",
           h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
   fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
   fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
   fprintf(stream, "------------------------------------------------------\n");
   for (i = 1; i <= num_stats; i++) {
      print_stats_line(h, stream, 100 * i / num_stats);
   }
   fprintf(stream, "------------------------------------------------------\n");
   fprintf(stream, "\n");
}
