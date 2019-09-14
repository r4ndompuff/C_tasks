#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    if (argv[1] == argv[2])
        exit(0);
    /* Error */
    if (argc != 3) {
        printf("Incorrect arguments. Exit.\n");
        exit(1);
    }
    FILE *f1, *f2;
    f1 = fopen(argv[1], "r");
    if (f1 == NULL) {
        /* Error */
        printf("Incorrect argument\n");
        exit(1);
    }
    f2 = fopen(argv[2], "r");
    if (f2 == NULL) {
        /* Error */
        printf("Incorrect argument\n");
        exit(1);
    }
    char c1, c2;
    size_t line_count = 1, char_count = 1;  // Symbol and lines counters
    c1 = fgetc(f1);
    c2 = fgetc(f2);

    while (!feof(f1) && !feof(f2)) {
        if (c1 != c2) {
            /* Different */
            printf("%s %s differ: char %zd, line %zd\n", argv[1], argv[2], char_count, line_count);
            exit(0);
        }
        if (c1 == '\n') {
            line_count++;
        }
        char_count++;
        c1 = fgetc(f1);
        c2 = fgetc(f2);
    }
    if (feof(f1) && feof(f2)){
        /* Same */
        printf("%s have no differs with %s\n", argv[1], argv[2]);
        exit(0);
    }
    /* No EOF's */
    printf("%s %s differ: char %zd, line %zd\n", argv[1], argv[2], char_count, line_count);
    exit(0);
    return 0;
}
