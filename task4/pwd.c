#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 16

int main() {
    char * dir;
    int attempt = 1;
    dir = calloc(N, sizeof(char));
    getcwd(dir, attempt++ * N);
    while (dir[0] == '\0'){
        dir = realloc(dir, N * attempt * sizeof(char));
        getcwd(dir, attempt++ * N);
    }
    printf("%s\n", dir);
    free(dir);
    return 0;
}
