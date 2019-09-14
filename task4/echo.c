#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char * argv[]) {
    int nflag;      /* If 1 there is no -n */
    /* Checking -n */
    if (*++argv && !strcmp(*argv, "-n")) {
        --argc;
        nflag = 1;
    } else {
        *--argv;
        nflag = 0;
    }

    while (--argc > 0)
        printf("%s%s", *++argv, (argc > 1) ? " " : "");
    /* If -n, then \n */
    if (!nflag)
        putchar('\n');
    return 0;
}
