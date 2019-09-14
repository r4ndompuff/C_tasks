#include <stdlib.h>
#include "defs.h"
#define SIZE 16

/* Cleaning the list */

void clear_list() {
    int i;
    sizelist = 0;
    curlist = 0;
    if (lst == NULL)
        return;
    for (i=0; lst[i] != NULL; i++)
        free(lst[i]);
    free(lst);
    lst= NULL;
}

/* Setting list to NULL  */

void null_list() {
    sizelist = 0;
    curlist = 0;
    lst = NULL;
}

/* 'Finishing' the list */

void termlist() {
    if (lst == NULL)
        return;
    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist+1)*sizeof(*lst));
    lst[curlist] = NULL;
    /* Fit memory to the size of the list */
    lst = realloc(lst, (sizelist = curlist+1)*sizeof(*lst));
}

/* Setting buf to NULL */

void nullbuf() {
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

/* Adding symbol to the word */

void addsym() {
    if (curbuf > sizebuf-1)
        buf = realloc(buf, sizebuf += SIZE); /* увеличиваем буфер при
                                              необходимости */
    buf[curbuf++] = c;
}

/* Checking on the error */

int check_error(int c) {
    char ok_alph[66] = {"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890$_/."};
    int count = 0;
    if (symset(c)) {
        while (count <= 66) {
            if ((char)c == ok_alph[count]) {
                return 0;
            }
            count += 1;
        }
    }
    else
        return 0;
    if (c != '\n') {
        while (c != '\n')
		fread(&c, 1, 1, stdin);
	fflush(stdin);
	termlist();
	clear_list();
	free(buf);
        erra = 1;
        return 1;
    }
printf("ASDASDASD");
    return 0;
}

/* Adding our word to the list */

void addword() {
    if (curbuf > sizebuf-1)
        buf = realloc(buf, sizebuf += 1); /* increasing for '\0' */
    buf[curbuf++] = '\0'; /* Making it fit the size */
    buf = realloc(buf, sizebuf = curbuf);
    if (curlist > sizelist-1)
        lst = realloc(lst, (sizelist += SIZE)*sizeof(*lst)); /* Increasing if needed */
    lst[curlist++] = buf;
}

/* Outputing the list */

void print_list(list l){
    if (l == NULL)
        return;
    while (*l != NULL)
        printf("%s\n",*l++);
}

/* Sorting the list in alphaber order */

void sort_list() {
    for (int i = curlist-1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            int k = 0;
            while ((lst[j][k] == lst [j + 1][k])
                   && (lst[j][k] != '\0')
                   && (lst[j+1][k] != '\0'))
                k++;
            if (lst[j][k] > lst[j + 1][k]) {
                char *tmp = lst[j];
                lst[j] = lst[j + 1];
                lst[j + 1] = tmp;
            }
        }
    }
    return;
}

/* Detecting if it is a special symbol or average */

int symset(int c){
    return c != '\n' &&
    c != ' ' &&
    c != '\t' &&
    c != '>' &&
    c != '&' &&
    c != '|' &&
    c != '<' &&
    c != ';' &&
    c != '(' &&
    c != ')' &&
    c != EOF;
}

/* Getting symbol from fread */

char getsym() {
    char oks[76] = "\n\t ><&;|()1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM$_/.";
    for (int i = 0; i < 76; i++)
        ok_ok[i] = oks[i];
    if (strcount < 2) {
        fread(str, sizeof(char), SIZE, stdin);
        strcount = SIZE;
    }
    strcount--;
    int count = 0;
    while (count <= 66) {
        if (str[SIZE-strcount-1] == ok_ok[count]) {
            return str[SIZE-strcount-1];
        }
        count += 1;
    }
    return '@';
}

/* Building the list.
 1) Detecting what symbol is it (special or average).
 2) If special, we detecting is it a pair symbol.
 3) Else if average, we adding it to the word.
 4) If it is a break (special) symbol, we push word to the list.
 5) Then starting again.
 */

list build_list() {
    typedef enum {Start, Word, Greater, Greater2, Newline, Stop,
        OtherSpec, Ampersand, Ampersand2, Stick, Stick2, Error} vertex;
    vertex V = Start;
    c = getsym();
    null_list();
    while (1 == 1)
        switch(V) {
                /*-------------------START-----------------*/
            case Start:
                if ((c == ' ') || (c == '\t'))
                    c = getsym();
                else if (c == EOF) {
                    termlist();
                    clear_list();
                    printf("\nSize of the list: %d\n", sizelist-1);
                    printf("Your result: \n");
                    V = Stop;
                }
                else if (c == '\n') {
                    termlist();
                    if (feof(stdin) == 0) {
                        printf("\nSize of the list: %d\n", sizelist-1);
                        printf("Your result: \n");
                    }
                    V = Newline;
                }
                else {
                    nullbuf();
                    addsym();
                    if (c == '>')
                        V = Greater;
                    else if (c == '|')
                        V = Stick;
                    else if (c == '&')
                        V = Ampersand;
                    else if ((c == ';') || (c == '<') || (c == '(') || (c == ')'))
                        V = OtherSpec;
                    else if (check_error(c) == 0)
                        V = Word;
                    else {
                        printf("ERROR! Wrong input! \n");
                        termlist();
                        clear_list();
                        V = Error;
		        strcount = SIZE;
                        printf("Try again.\n\n");
                        break;
                    }
                    c = getsym();
                }
                break;
                /*--------------------WORD-----------------*/
            case Word:
                if (check_error(c) == 0) {
                    if (symset(c)) {
                        addsym();
                        c = getsym();
                    }
                    else {
                        V = Start;
                        addword();
                    }
                }
                else {
                    printf("ERROR! Wrong input! \n");
                    termlist();
                    clear_list();
		    strcount = SIZE;
                    printf("Try again.\n\n");
                    V = Error;
                }
                break;
                /*-------------------->_>>-----------------*/
            case Greater:
                if (c == '>') {
                    addsym();
                    c = getsym();
                    V = Greater2;
                }
                else {
                    V = Start;
                    addword();
                }
                break;
            case Greater2:
                V = Start;
                addword();
                break;
                /*--------------------|_||-----------------*/
            case Stick:
                if (c == '|') {
                    addsym();
                    c = getsym();
                    V = Stick2;
                }
                else {
                    V = Start;
                    addword();
                }
                break;
            case Stick2:
                V = Start;
                addword();
                break;
                /*--------------------&_&&------------------*/
            case Ampersand:
                if (c == '&') {
                    addsym();
                    c = getsym();
                    V = Ampersand2;
                }
                else {
                    V = Start;
                    addword();
                }
                break;
            case Ampersand2:
                V = Start;
                addword();
                break;
                /*-----------------Other_Spec----------------*/
            case OtherSpec:
                addword();
                V = Start;
                break;
                /*------------------New_line-----------------*/
            case Newline:
                return lst;
                break;
                /*--------------------Stop-------------------*/
            case Stop:
                exit(0);
                break;
            case Error:
                termlist();
                clear_list();
                return NULL;
                break;
        }
}
