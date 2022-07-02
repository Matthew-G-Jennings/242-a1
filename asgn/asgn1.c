#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

/**
 * Main runnable program for the interaction with the hashtable.
 * Reads from stdin and takes command line arguments as specified
 * using -h.
 *
 * @author Jeremy Shanks, Will Johnstone, Matthew Jennings
 */

#define DEFAULT_TABLE_SIZE 113
#define DEFAULT_SNAPSHOT 10

/**
 * Prints a help message describing how to use the program to stdout.
 */
static void help(){

    printf("Usage: ./asgn1 [OPTION]... <STDIN>\n");
    printf("\n");
    printf("Perform various operations using a hash table.  By default, words");
    printf(" are\n");
    printf("read from stdin and added to the hash table, before being");
    printf(" printed out\n");
    printf("alongside their frequencies to stdout.\n");
    printf("\n");
    printf("-d\t\tUse double hashing (linear probing is the default)\n");
    printf("-e\t\tDisplay entire contents of hash table on stderr\n");
    printf("-p\t\tPrint stats info instead of frequencies & words\n");
    printf("-s SNAPSHOTS\tShow SNAPSHOTS stats snapshots (if -p is used)\n");
    printf("-t TABLESIZE\tUse the first prime >= TABLESIZE as htable size\n");
    printf("\n");
    printf("-h\t\tDisplay this message\n");
    printf("\n");
}

/**
 * Initilizes the hashtable and takes input from stdin.
 *
 * Command line argument interpretation happens here.
 *
 * Performs actions based on the given arguments.
 *
 * @param argc the number of arguments passed to the program
 * @param argv a two dimensional character array with that hold the arguements
 * passed to the program
 *
 * @return EXIT_SUCCESS if successful EXIT_FAILURE if not
 */
int main(int argc, char **argv) {

    hashing_t collision_resolution_strategy = LINEAR_P;
    htable h;
    char option;
    char word[256];
    const char *optstring = "deps:t:h";

    /* flags used to see which command line arguements exist */
    int e, p, s, snapshot, t, tablesize;

    e = 0;
    p = 0;
    s = 0;
    snapshot = -1;
    t = 0;
    tablesize = -1;

    /*  this will read the whole command line argument used, for example if one
        uses "./asgn1 -d -e -p -h"
        the while loop ensures that all "-d", "-e", "-p", "-h" will be used. */
    while((option = getopt(argc, argv, optstring)) != EOF) {
        switch(option) {
            case 'd':
                /* switches the collision resolution strategy from LINEAR_P to
                   DOUBLE_H */
                collision_resolution_strategy = DOUBLE_H;
                break;

            case 'e':
                /* triggers the e flag to be true */
                e = 1;
                break;

            case 'p':
                /* triggers the p flag to be true */
                p = 1;
                break;

            case 's':
                /* triggers the s flag to be true */
                s = 1;
                /* this converts the string optarg to an int and writes
                   that value to snapshot */
                sscanf(optarg, "%d", &snapshot);
                break;

            case 't':
                /* triggers the t flag to be true */
                t = 1;
                /* this converts the string optarg to an int and writes that
                   value to tablesize */
                sscanf(optarg, "%d", &tablesize);
                break;

            case 'h':
                help();
                exit(EXIT_SUCCESS);

            default:
                help();
                exit(EXIT_FAILURE);
        }
    }

    /* default case for our hashtable here, default size 113 */
    if (t != 1){
        tablesize = DEFAULT_TABLE_SIZE;
    }

    /* if the t flag has been triggered and a sensible argument has been
    given */
    if(t == 1 && tablesize != -1){
        if(tablesize == 1 || tablesize == 0){
            tablesize = 2;
        } else {
            tablesize = gen_prime(tablesize);
        }
    }

    h = htable_new(collision_resolution_strategy, tablesize);

    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }

    if(e == 1){
        htable_print_entire_table(h, stderr);

        if(p == 0){
            htable_print(h, stdout);
        }
    }

    if(s == 1 && snapshot != 0 && p == 1){
        htable_print_stats(h, stdout, snapshot);
    } else if (p == 1){
        htable_print_stats(h, stdout, DEFAULT_SNAPSHOT);
    }

    htable_free(h);

    return EXIT_SUCCESS;
}
