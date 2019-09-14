#ifndef EXEC_H
#define EXEC_H

#include <unistd.h>
#include "tree.h"

typedef struct backgrndList {   // Structure of zombie list
    int pid;
    struct backgrndList *next;
} intlist;

int execute_tree(tree);         // Exec cmds from the tree
void clear_zombie(intlist *);   // Cleaning zombie list
#endif
