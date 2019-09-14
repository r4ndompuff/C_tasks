#include <stdio.h>
#include <stdlib.h>
#define strsize 20

typedef struct Node {                       // List structure
    char *elem;
    struct Node *next;
} node;                                  // Node is a list structure
typedef struct Node * link;
typedef link pointer;                  // Point on first element of list

/* Functions desc */

pointer addel (char[], pointer);
pointer rm_same(pointer, char[]);

int wrdcmp(char[], char[]);
void printli(pointer);
void dellis(pointer l);

/* Let's do the job */

/* Searching the end of list and then copy string to the list.elem */

pointer addel(char y[strsize], link l) {
    if (l == NULL) {
        l = malloc(sizeof(node));
        int siz = 0;
        l->elem = (char *)malloc(siz);
        for (int i = 0; i < strsize; i++)
            if (y[i] != '\0') {
                siz++;
                l->elem = (char *)realloc(l->elem, siz);
                l->elem[i] = y[i];
            }
            else {
                l->next = NULL;
                l->elem[i] = '\0';
                return l;
            }
        l->next = NULL;
    } 
    else
        l->next = addel(y, l->next);
   return l;
}

/* Going deeper until last element and checking on same elements */

pointer rm_same(pointer l, char s[]) {
    if (l->next != NULL) {
        if (wrdcmp(l->elem, s) == 1) {
            pointer q;
            q = l;
            l = l->next;
            free(q);
            l = rm_same(l, s);
        } 
        else
            l->next = rm_same(l->next, s);
    }
    return l;
}

/* If everything is alright, then return 1, else return 0 */

int wrdcmp(char a[strsize], char b[strsize]) {
    int i = 0;
    while ((a[i] != '\0') || (b[i] != '\0')) {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    if ((a[i] != '\0') || (b[i] != '\0'))
        return 0;
    return 1;
}

/* Until it is the end, we printing elements without '\0' */

void printli(pointer l) {
    if (l != NULL) {
        int i = 0;
        while (l->elem[i] != '\0')
            printf("%c", l->elem[i++]);
        printf(" ");
        printli(l->next);
    }
}

/* Until the end we drop the bridge to the next island and burn previous!!! */

void dellis(pointer l) {
    pointer q;
    while (l != NULL) {
        q = l;
        l = l->next;
        free(q->elem);
        free(q);
    }
}
 

int main() {
    pointer L = NULL;
    char Y[strsize];
    char last_elem[strsize];    // Last element
    int count = 0;
    printf("Enter words:\n");
    while (scanf("%s", Y) != EOF) {
             L = addel(Y, L);
             count++;
    }
    if (count == 0) {
        printf("\nEmpty list.\n");
        return 0;
    }

    /* Last element searching */
    pointer P = L;            // Additional pointer
    while (P->next != NULL)
        P = P->next;
    int i = 0;
    while (P->elem[i] != '\0') {
        last_elem[i] = P->elem[i];
        i++;
    }
    last_elem[i] = '\0';

    L = rm_same(L, last_elem);

    printf("\nYour list: \n");
    printli(L);
    printf("\n");
    dellis(L);
    system("read");
    return 0;
}
