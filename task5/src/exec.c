#include "../include/exec.h"
#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 5

int exec_conv(tree);            // Making Conv
int exec_cd(char * argv[]);     // cd
int exec_pwd(tree);             // pwd
int exec_cmd (tree);            // Creating procces for cmd
void add_zombie(int);           // List of all zombies
void kill_process(void);        // Process killer
extern int EOF_flag;

int start_pid;                  // Id active process
intlist * intL = NULL;          // Zombie list

/*-------------------------------Cmd executor from the tree--------------------------------------------*/

int execute_tree (tree T) {
    int res = 0;
    tree P = T;
    start_pid = getpid();
    /* While tree is not */
    while (P != NULL) {
        res = exec_conv(P);
        if ((P->type == AND) && (res != 0)) {
            while ((P->next != NULL) && (P->type == AND)) {
                    P = P->next;
            }
        }
        else if ((P->type == OR) && (res == 0)) {
            while ((P->next != NULL) && (P->type == OR)) {
                P = P->next;
            }
        }
        P = P->next;
    }
    return res;
}

/*----------------------------------------------cd---------------------------------------*/

int exec_cd(char *argv[]) {
    char *s;
    if (argv[1] == NULL) {
        s = getenv("HOME");
        if (s == NULL) {
            fprintf(stderr, "cannot find home folder! :c\n");
            return 1;
        }
        else
            chdir(s);
    }
    /* Searching for errors */
    else if (argv[2] != NULL)  {
        fprintf(stderr, "cd cannot have more then one argument!\n");
        return 1;
    }
    /* Searching for errors */
    else if (chdir(argv[1])) {
        perror(argv[1]);
        return 1;
    }
    return 0;
}

/*----------------------------------------------pwd---------------------------------------*/

int exec_pwd(tree T) {
    char * dir;
    int attempt = 1;
    /* Searching for errors */
    if (T->argv[1] != NULL) {
        fprintf(stderr, "pwd cannot have arguments!\n");
        return 1;
    }
    /* Catalog to dir */
    dir = calloc(SIZE, sizeof(char));
    getcwd(dir, attempt++ * SIZE);
    while (dir[0] == '\0') {
        dir = realloc(dir, SIZE * attempt * sizeof(char));
        getcwd(dir, attempt++ * SIZE);
    }
    fprintf(stdout, "%s\n", dir);
    free(dir);
    exit(0);
}

/*----------------------------------------------Process killer---------------------------------------*/

void kill_process(void) {
    int pid;
    pid = getpid();
    if (start_pid != pid)
    	exit(9);
}

/*------------------------------------------List of all zombies---------------------------------------*/

void add_zombie(int pid) {
    intlist * tmp;
    tmp = malloc(sizeof(intlist));
    tmp->pid = pid;
    tmp->next = intL;
    intL = tmp;
}

/*----------------------------------------------Zombie killer---------------------------------------*/

void clear_zombie(intlist * intL) {
    int status;
    intlist * tmp;
    while (intL != NULL) {
        waitpid(intL->pid, &status, 0);
        tmp = intL;
        intL = intL->next;
        free(tmp);
    }
    return;
}
