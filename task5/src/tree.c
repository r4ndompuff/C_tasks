#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 5

char * plex;                        // Actual lex
int word_count = 0;                 // Word index
int brackets_count = 0;           
int * commands_in_brackets = NULL; 
int error_flag = 0;                   // 1, if error

extern void error(int, char *message, list *l); 
static tree make_cmd(void);             // Creating tree from 1 element
static void add_arg(tree t);           
static void bracket_manipulation(void); // Adding memory for commands_in_brackets
extern void clear_brackets(void);       // Clear memory
extern void error_prepare(void);        // Nulling index variables


/*----------------------------------------------Tree builder---------------------------------------*/

tree build_tree(list l) {
    tree begin_cmd; 
    tree prev_cmd;  
    tree cur_cmd;   
    // if none in cycle then go back
    if ((l == NULL) || ((plex = l[word_count++]) == NULL)) {
        word_count = 0;
        return NULL;
    }
    begin_cmd = cur_cmd = make_cmd();
    prev_cmd = cur_cmd;
    /* Searching for errors */
    if ((strcmp(plex, ")") == 0) || (strcmp(plex, "<") == 0) || (strcmp(plex, ">") == 0) || (strcmp(plex, ">") == 0) || (strcmp(plex, ";") == 0) || (strcmp(plex, "&") == 0) || (strcmp(plex, "||") == 0) || (strcmp(plex, "&&") == 0) || (strcmp(plex, "|") == 0)) {
        error_prepare();
        error(2, "Unexpected symbol.", &l);
        return cur_cmd;
    }
        /* If '(', then subtree */
    if (strcmp(plex, "(") == 0) {
        /* Searching for errors */
        if (l[word_count] == NULL) {
            error_prepare();
            error(2, "Missing )", &l);
            return cur_cmd;
        }
        bracket_manipulation();
        cur_cmd = build_tree(l);
        prev_cmd->psubcmd = cur_cmd;
        cur_cmd = prev_cmd;
    } else {
       if ((brackets_count > 0) && (cur_cmd->argv == NULL))
           commands_in_brackets[brackets_count - 1]++;
        add_arg(cur_cmd);
    }
    plex = l[word_count++];
    while ((plex != NULL) && (error_flag == 0)) {
        /* If nothing after ||, &&, |, then error*/
        if (((strcmp(plex, "||") == 0) || (strcmp(plex, "&&") == 0) || (strcmp(plex, "|") == 0)) && (l[word_count] == NULL)) {
            error_prepare();
            error(2, "Missing command after ||, && or |.", &l);
            return cur_cmd;
        }
        /* Searching for errors */
        if (strcmp(plex, "(") == 0) {
            error_prepare();
            error(2, "No command separator before (.", &l);
            return cur_cmd;
        }
        /* If ')', then go back */
        else  if (strcmp(plex, ")") == 0) {
            /* Searching for errors */
            if ((brackets_count != 0) && (commands_in_brackets[brackets_count - 1] == 0)) {
                error_prepare();
                error(2, "No commands in brackets.", &l);
                return cur_cmd;
            } else if (brackets_count == 0)  {
                error_prepare();
                error(2, "Missing (.", &l);
                return cur_cmd;
            }
            /* MEMORY CLEAN!!! */
            if ((brackets_count > 0) && (commands_in_brackets[brackets_count - 1] > 1)) {
                commands_in_brackets[brackets_count - 1]--;
                return begin_cmd;
            } else if ((brackets_count > 0) && (commands_in_brackets[brackets_count - 1] == 1)) {
                commands_in_brackets[brackets_count - 1] = 0;
                commands_in_brackets = realloc(commands_in_brackets, ((brackets_count - 1) * sizeof(int)));
                if (brackets_count == 1)
                    free(commands_in_brackets);
                brackets_count--;
                return begin_cmd;
            }
        }
        /* If '<', then add input file */
        else if (strcmp(plex, "<") == 0) {
            /* Error, if no file */
            if (l[word_count] == NULL)  {
                error_prepare();
                error(2, "Nothing after <.", &l);
                return cur_cmd;
            }
            plex = l[word_count++];
            /* Error, if two files */
            if (cur_cmd->infile != NULL) {
                error_prepare();
                error(2, "Only one input file allowed.", &l);
                return cur_cmd;
            }
            /* Adding file to the tree */
            long tmp = strlen(plex);
            cur_cmd->infile= calloc(tmp + 1, sizeof(char));
            strncpy(cur_cmd->infile, plex, tmp + 1);
            plex = l[word_count++];
        }
        /* If '>', then add output file */
        else if ((strcmp(plex, ">") == 0) || (strcmp(plex, ">>") == 0)) {
            /* Error, if two files */
            if (cur_cmd->outfile != NULL) {
                error_prepare();
                error(2, "Only one output file allowed.", &l);
                return cur_cmd;
            }
            /* Error, if no file */
            if (l[word_count] == NULL)  {
                error_prepare();
                error(2, "Nothing after > (>>).", &l);
                return cur_cmd;
            }
            if (strcmp(plex, ">>") == 0)
                cur_cmd->append = 1;
            /* Adding file to the tree */
            plex = l[word_count++];
            long tmp = strlen(plex);
            cur_cmd->outfile= calloc(tmp + 1, sizeof(char));
            strncpy(cur_cmd->outfile, plex, tmp + 1);
            plex = l[word_count++];
        }
        /* if '|', then new cmd in tree */
        else if (strcmp(plex, "|") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->pipe = cur_cmd;
            if (cur_cmd->backgrnd == 1)
                prev_cmd->backgrnd = 1;
            if (cur_cmd->next != NULL) {
                prev_cmd->next = cur_cmd->next;
                cur_cmd->next = NULL;
                prev_cmd->type = cur_cmd->type;
                cur_cmd->type = NXT;
            }
            prev_cmd = cur_cmd;
        }
        /* If '||', then new cmd in tree */
        else if (strcmp(plex, "||") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = OR;
            prev_cmd = cur_cmd;
        }
        /* If '&&', then new cmd in tree */
        else if (strcmp(plex, "&&") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = AND;
            prev_cmd = cur_cmd;
        }
        /* If ';', then new cmd in tree */
        else if (strcmp(plex, ";") == 0) {
            if ((brackets_count > 0) && (l[word_count] != NULL) && (l[word_count][0] == ')')) {
                plex = l[word_count++];
                continue;
            }
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = NXT;
            prev_cmd = cur_cmd;
        }
        /* If '&', then make_backgrnd(begin_cmd) */
        else if (strcmp(plex, "&") == 0) {
            cur_cmd->backgrnd = 1;
            if ((brackets_count > 0) && (l[word_count] != NULL) && (l[word_count][0] == ')')) {
                plex = l[word_count++];
                continue;
            }
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = NXT;
            prev_cmd = cur_cmd;
        }
        else {
            /* Searching for errors */
            if ((cur_cmd->infile != NULL) || (cur_cmd->outfile != NULL) || (cur_cmd->psubcmd != NULL)) {
                error_prepare();
                error(2, "Unexpected lexem.", &l);
                return cur_cmd;
            }
            add_arg(cur_cmd);
            plex = l[word_count++];
        }
    }
    /* Searching for errors */
    if ((brackets_count != 0) && (plex == NULL)) {
        error_prepare();
        error(2, "Sorry, but looks like you miss \')\'", &l);
        return cur_cmd;
    }
    word_count = 0;
    return begin_cmd;
}

/*----------------------------------------------Counting cmd in bracketsr---------------------------------------*/

void bracket_manipulation(void) {
    brackets_count++;
    if (brackets_count == 1)
        commands_in_brackets = calloc(1, sizeof(int));
    else
        commands_in_brackets = realloc(commands_in_brackets, (brackets_count * sizeof(int)));
    commands_in_brackets[brackets_count - 1] = 0;
    if (brackets_count > 1)
        commands_in_brackets[brackets_count - 2]++;
}

/*---------------------------------Making tree from 1 element and NULL everything------------------------------------*/

tree make_cmd() {
    tree t = NULL;
    t = calloc(1, sizeof(*t));
    t->argv = NULL;
    t->infile = NULL;
    t->outfile = NULL;
    t->append = 0;
    t->backgrnd = 0;
    t->psubcmd = NULL;
    t->pipe = NULL;
    t->next = NULL;
    return t;
}

/*--------------------------------------------Adding element to argv of cur cmd---------------------------------------*/

void add_arg(tree t) {
    int i = 0;
    if (t->argv == NULL)
        t->argv = calloc(1, sizeof(*t->argv));
    while (t->argv[i] != NULL)
        i++;
    long tmp = strlen(plex);
    t->argv[i] = calloc(tmp + 1, sizeof(char));
    strncpy(t->argv[i++], plex, tmp + 1);
    t->argv = realloc(t->argv, ((i + 1) * sizeof(*t->argv)));
    t->argv[i] = NULL;
}

/*----------------------------------------------Addit. func for tree printer---------------------------------------*/

void make_shift(int n) {
    while (n--)
        putc(' ', stderr);
}

/*----------------------------------------------Addit. func for tree printer---------------------------------------*/

void print_argv(char **p, int shift) {
    char **q = p;
    if (p != NULL) {
        while (*p != NULL) {
            make_shift(shift);
            fprintf(stderr, "argv[%d]=%s\n",(int) (p - q), *p);
            p++;
        }
    }
}

/*----------------------------------------------Tree printer---------------------------------------*/

void print_tree(tree t, int shift) {
    char **p;
    if (t == NULL)
        return;
    p = t->argv;
    if (p != NULL)
        print_argv(p, shift);
    else {
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if (t->infile == NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if (t->outfile == NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", t->type == NXT ? "NXT": t->type ==OR ? "OR": "AND");
    make_shift(shift);
    if(t->psubcmd == NULL)
        fprintf(stderr, "psubcmd=NULL \n");
    else {
        fprintf(stderr, "psubcmd---> \n");
        print_tree(t->psubcmd, shift + 5);
    }
    make_shift(shift);
    if(t->pipe == NULL)
        fprintf(stderr, "pipe=NULL \n");
    else {
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift + 5);
    }
    make_shift(shift);
    if(t->next == NULL)
        fprintf(stderr, "next=NULL \n");
    else {
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift + 5);
    }
}

/*----------------------------------------------Clearing tree and free memory---------------------------------------*/

void clear_tree(tree T) {
    if (T == NULL)
        return;
    clear_tree(T->next);
    clear_tree(T->pipe);
    clear_tree(T->psubcmd);
    free(T->infile);
    free(T->outfile);
    delete_list(T->argv);
    free(T);
    T = NULL;
}
