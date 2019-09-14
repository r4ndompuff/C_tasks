#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <libgen.h>

#define LEN 30

int main(int argc, char *argv[]) {
    /* Argument check */
    if (argc != 3)
        printf("Incorrect number of arguments.\n");
    else {
        char buf[LEN];
        struct stat info1, info2;
        int f1, f2, check;
        stat(argv[1], &info1);
        stat(argv[2], &info2);
        /* Atgument check */
        if (info1.st_ino == info2.st_ino) {
            printf("The arguments are the same file.\n");
            exit(-1);
        }
        if ((f1 = open(argv[1], O_RDONLY, 0)) == -1) {
            printf("Can't read the file.");
            exit(-1);
        }
        if (S_ISDIR(info2.st_mode)) {
            strcat(argv[2], "/");
            strcat(argv[2], basename(argv[1]));
            if ((f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, info1.st_mode)) == -1) {
                printf("Can't open file for writing.\n");
                exit(-1);
            }
        }
        else if (f2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, info1.st_mode) == -1) {
            printf("Can't open file for writing.\n");
            exit(-1);
        }
        while ((check = read(f1, buf, LEN)) > 0)
            write(f2, buf, check);
        close(f1);
        close(f2);
    }
    return 0;
}