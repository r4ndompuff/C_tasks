#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv) {
  DIR *dptr;
  struct dirent *ds;
  dptr = opendir(".");
  while ((ds = readdir(dptr)) != 0) {
    if (!strcmp(ds->d_name, ".") || !strcmp(ds->d_name, ".."))
      continue; /* Skip himself and parents */
    printf("%s\n", ds->d_name);
  }
  closedir(dptr);
  return 0;
}
