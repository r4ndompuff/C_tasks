#ifndef tree_h
#define tree_h

#include "list.h"

enum type_of_next{
    NXT, AND, OR   // Type of cmd connections between each other
};

struct cmd_inf {
    list argv;      // List of comand and args
    char *infile;   // Input
    char *outfile;  // Output
    int append;     // =1, if >>
    int backgrnd;   // =1, if it neede to background
    struct cmd_inf* psubcmd; // cmd for subshell
    struct cmd_inf* pipe; // Next cmd after '|'
    struct cmd_inf* next; // Next cmd after ';' or '&'
    enum type_of_next type;// Link with next cmd by ; or && or ||
};

typedef struct cmd_inf *tree;
typedef struct cmd_inf node;

tree build_tree(list);    
void print_tree(tree, int);
void clear_tree(tree); 

#endif
