#include <stdio.h>
#include <stdlib.h>

typedef struct Tree_node {              // Tree structure
    int data;
    struct Tree_node *left;
    struct Tree_node *right;
} t_node;                           // t_node is "record"
typedef struct Tree_node *tree_point;
typedef tree_point Pointer;       // Pointer to root

/* Functions desc */

Pointer addel (int, Pointer);
Pointer rmv (int, Pointer);
Pointer minel(Pointer);
Pointer check (int, Pointer);
int str2dig(char[]);      
void del_tree(Pointer); 

/* Let's do the job. */   

int str2dig(char td[]) {
    int k = 1;
    int d = 0;
    while (td[k] != '\0') {
        d = d * 10 + ((int)td[k] - (int)('0'));
        k += 1;
    }
    return d;
}

/* Going deeper, until we found specialized place for our element,
   with NULL-pointers. */

Pointer addel(int dig, Pointer t) {
    if (t == NULL) {
        t = malloc(sizeof(t_node));
        t->data = dig;
        t->left = NULL;
        t->right = NULL;
    } 
    else  if (dig < t->data)
        t->left = addel(dig, t->left);
    else if (dig > t->data)
        t->right = addel(dig ,t->right);
    return t;
}

/* Going deeper, until we found our element,
   then if we have 2 sub-trees, we are 
   searching min element from right sub-tree
   and change (delete) our needed for delete data on
   min(right_subtree) and then deleting it.
   If we have only 1 sub-tree, then just replacing it. */

Pointer rmv(int dig, Pointer t) {
    if (t != NULL) {
        if (dig < t->data)
            t->left = rmv(dig, t->left);
        else if (dig > t->data)
            t->right = rmv(dig, t->right);
        else if ((t->left != NULL) && (t->right != NULL)) {
            t->data = minel(t->right)->data;
            t->right = rmv(t->data, t->right);
        } 
        else if (t->left != NULL) {
            t = t->left;
        } 
        else if (t->right != NULL) {
            t = t->right;
        } 
        else if ((t->left == NULL) && (t->right == NULL))
            if (t->data == dig) {
                t = NULL;
                free(t);
            }
    }
    return t;
}

/* Easy. */

Pointer minel(Pointer t) {
    if (t->left == NULL)
        return t;
    return minel(t->left);
}

/* If we found or came to end => STOP
   Other part is just a searching. */

Pointer check(int y, Pointer t) {
    if ((t == NULL) || (y == t->data))
        return t;
    else if (y < t->data)
        return check(y, t->left);
    else
        return check(y, t->right);
}

/* Understandable. */

void del_tree(Pointer t) {
    if(t != NULL) {
        del_tree(t->left);
        del_tree(t->right);
        free(t);
    }
}

int main() {
    Pointer T = NULL;  // "Created" tree
    int dgt;
    char c;
    char S[20];
    printf("Type your tree-commands: \n");
    while (scanf("%s", S) != EOF) {
        c = S[0];
        dgt = str2dig(S);
        if (dgt <= -1) {
            printf("\nError.\n");
            break;
        }
        if (c == '+') {
            T = addel(dgt, T);
        } 
        else if (c == '-') {
            T = rmv(dgt, T);
        } 
        else if (c == '?') {
            Pointer check_res = NULL;
            check_res = check(dgt, T);
            printf ("\nTree include %d? ", dgt);
            if ((check_res == NULL) || (check_res->data != dgt))
                printf("No\n");
            else if (check_res->data == dgt)
                printf("Yes\n");
        }
    }
    del_tree(T);
    system("read");
    return 0;
}