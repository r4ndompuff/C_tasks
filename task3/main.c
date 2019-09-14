#include <stdlib.h>
#include "defs.h"
#define SIZE 16

int main() {
    erra = 0;
    strcount = SIZE;
    null_list();
    while (feof(stdin) == 0) {
        for (int i = 0; i < 76; i++)
            str[i] = '\n';
        strcount = SIZE;
        printf("Type in your sentence: \n");
        fread(str, sizeof(char), SIZE, stdin);
        lst = build_list();
        if (erra == 0) {
            print_list(lst);
            if (feof(stdin))
                printf("Finishing...");
            else
                printf("\nYour sorted list: \n");
            sort_list(lst);
            print_list(lst);
            clear_list();
            printf("\n");
        }
	clear_list();
        erra = 0;
    }
    clear_list();
    return 0;
}
