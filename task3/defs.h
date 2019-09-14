#ifndef __LIST__
#define __LIST__
#include <stdio.h>
#define SIZE 16

typedef char **list;
typedef void *(*vertex)(void);

list lst; /* List of words */
char *buf; /* Buffer for accumulating the word */
int c; /* Symbol */
int sizebuf; /* Size of current word (Buf size) */
int sizelist; /* Size of whole list */
int curbuf; /* Index of current symbol in buf */
int curlist; /* Index of current word in list */
int erra; /* Flag for Error */
char str[SIZE]; /* String from fread */
int strcount; /* Counter for getting symbol from string */
char ok_ok[76]; /* All okay symbols, except eof */

list build_list(void); /* Building the list of words */
int check_error(int c); /* Check if we have wrong symbol */
int symset(int c); /* Check if it is not a special symbol */
char getsym(); /* Getting symbold from fread */
void null_list(void); /* List = NULL, sizelist and curlist set to 0 (zero) */
void nullbuf(void); /* Buf = NULL, sizebuf and curbuf set to 0 (zero) */
void addsym(void); /* Adding symbol to the word */
void addword(void); /* Adding word to the list */
void clear_list(void); /* Free the memory, sizelist and curlist set to 0 (zero) */
void termlist(void); /* Finishing the list: adding NULL on position of curlist */
void sort_list();  /* Sorting the list */
void print_list(list); /* Printing the list */

#endif
