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

extern int exec_conv(tree);            // Making Conv
extern int exec_cd(char * argv[]);     // cd
extern int exec_pwd(tree);             // pwd
extern int exec_cmd (tree);            // Creating procces for cmd
extern void add_zombie(int);           // List of all zombies
extern void kill_process(void);        // Process killer
extern int EOF_flag;

extern int start_pid;                  // Id active process
extern intlist * intL;                 // Zombie list

/*-----------------------------------Exec conv cmd from the tree---------------------------------------*/

int exec_conv(tree T) {
    tree P = T;
    tree Q = T;
    int pid = 0;
    int fd[2], old = -1;
    int status = 0;         // Finish flag
    signal(SIGINT, kill_process);
    while (P != NULL) {
        /* arv[0] = cd? */
        if ((P->argv != NULL) && (strcmp(P->argv[0], "cd") == 0)) {
            if (old != -1)
                close(old);
            /* Last cmd */
            if (P->pipe != NULL) {
                if (pipe(fd) < 0)
                    exit(1);
                old = fd[0];
                close(fd[1]);
            }
            status = exec_cd(T->argv);
            return status;
        }
        /* argv[0] = exit? */
        else if ((P->argv != NULL) && (strcmp(P->argv[0], "exit") == 0)) {
            if (getpid() != start_pid) {
                if (T->argv[1] != NULL) {
                    fprintf(stderr, "\nexit cannot have arguments! Nevermind, I will be exiting anyway.");
                    exit(1);
                }
                exit(0);
            } else {
                EOF_flag = 1;
                if (T->argv[1] != NULL) {
                    fprintf(stderr, "\nexit cannot have arguments! Nevermind, I will be exiting anyway.");
                    return 1;
                }
                return 0;
            }
        }
        /* First cmd */
        if (P == T) {
            if (P->pipe != NULL) {
                if (pipe(fd) < 0)
                    exit(1);
                if ((pid = fork()) == 0) {
                    dup2(fd[1], 1);
                    close(fd[0]);
                    close(fd[1]);
                    exec_cmd(P);
                }
                old = fd[0];
                close(fd[1]);
            }
            else if ((pid = fork()) == 0)
                exec_cmd(P);
        }
        else if (P->pipe == NULL) {
            if ((pid = fork()) == 0) {
                dup2(old, 0);
                close(old);
                exec_cmd(P);
            }
            close(old);
        }
        else {
            if (pipe(fd) < 0)
                exit(1);
            if ((pid = fork()) == 0) {
                dup2(old, 0);
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                close(old);
                exec_cmd(P);
            }
            close(fd[1]);
            close(old);
            old = fd[0];
        }
        Q = P;
        P = P->pipe;
    }
    while (wait(&status) != -1);
    return status;
}

/*----------------------------------------------Single cmd executor---------------------------------------*/

int exec_cmd(tree T) {
    int in, out = 0, res = 0;
    if (T->backgrnd == 1)
        signal(SIGINT, SIG_IGN);
    else
       signal(SIGINT, SIG_DFL);
    if  (T->infile != NULL) {
        if ((in = open(T->infile, O_RDONLY)) < 0) {
            fprintf(stderr, "%s: Input file does not exist!\n", T->infile);
            exit(1);
        }  
        dup2(in, 0);
        close(in);
    }
    /* Input source */
    if  (T->outfile != NULL) {
        if (T->append == 1) {
            if ((out = open(T->outfile, O_WRONLY | O_CREAT |O_APPEND, 0777)) < 0)
                exit(1);
        }
        else {
            if ((out = open(T->outfile, O_WRONLY | O_CREAT |O_TRUNC, 0777)) < 0)
                exit(1);
        }
        dup2(out, 1);
        close(out);
    }
    /* Background */
    if (T->backgrnd == 1) {
        int f = open("/dev/null", O_RDONLY);
        dup2(f, 0);
        if (fork() == 0) {
            execvp(T->argv[0], T->argv);
            exit(0);
        }
        else
            kill(getpid(), SIGKILL);
    }
    /* Subshell */
    if (T->psubcmd != NULL) {
        res = execute_tree(T->psubcmd);
        if (res != 0)
            exit(1);
        else
            exit(0);
    }
    /* arv[0] = pwd? */
    else if ((T->argv != NULL) && (strcmp(T->argv[0], "pwd") == 0)) {
        res = exec_pwd(T);
        exit(res);
    }
    else {
        execvp(T->argv[0], T->argv);
        fprintf(stderr, "%s: command  does not found!\n", T->argv[0]);
        exit(1);
    }
}