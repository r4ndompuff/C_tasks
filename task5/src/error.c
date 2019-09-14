#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "../include/list.h"
#include "../include/exec.h"

extern int brackets_count;
extern int * commands_in_brackets;
extern int error_flag;
extern int word_count;


/*----------------------------------------------Error holder--------------------------------------*/

void error(int n, char * message, list *l){
    /* If n = 1, it is lexem error */
    l = l + 1;
    if (n == 1) {
        printf("\nError in lexems. Please try again.\n");
        return;
        /* If n = 2, it is synt error */
    } else if (n == 2)
        printf("Syntax error. %s\n", message);
}

/*-------------------------------------If error -> free memory for brackets---------------------------------------*/

void clear_brackets(void) {
    if (brackets_count != 0) {
        brackets_count = 0;
        commands_in_brackets = realloc(commands_in_brackets, 0);
        free(commands_in_brackets);
    }
}

/*----------------------------------------Counter null and ticking error flag---------------------------------------*/

void error_prepare(void) {
    word_count = 0;
    clear_brackets();
    error_flag = 1;
}