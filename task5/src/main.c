#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#include "../include/list.h"
#include "../include/tree.h"
#include "../include/exec.h"

extern list lst;
extern int EOF_flag;
extern int error_flag;
extern intlist * intL;
sigjmp_buf jmper;
tree t = NULL;

/*----------------------------------------------Jump after ctrl+c---------------------------------------*/

void prep_jump(void) {
    lst = NULL;
    t = NULL;
    intL = NULL;
    printf("\n");
    siglongjmp(jmper, 1);
}

/*----------------------------------------------Main---------------------------------------*/

int main(int argc, char * argv[]) {
    argc++;
    argv++;
    while (1 != 2) {
        sigsetjmp(jmper, 1);            // Point of jump after ctrl+c
        error_flag = 0;                   
        delete_list(lst);               // List clean
        clear_tree(t);  
        clear_zombie(intL);  
        if (EOF_flag != 1)         
            printf("r4ndom:$ ");
        signal(SIGINT, prep_jump);          
        build_list();               
        if (EOF_flag == 1)             // If EOF than finish
            break;
        t = build_tree(lst);       
        if (error_flag == 0) {            // If no errors
            //print_tree(t, 1);
            execute_tree(t);
        }
    }
    signal(SIGINT, SIG_DFL);            // Returning normal signal
    clear_zombie(intL);
    return 0;
}

