#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int str_count = 1;
    int position, i, flag_value, delta, str;
    char c;
    char flag[32];
    FILE *f = fopen(argv[1], "r");

    if (argc <= 1) {
        printf("Wrong command! Ending...\n");
        exit(1);
    }

    if (argc == 3) /* If there is a flag */
    {
        strcpy(flag, argv[2]);
        for (i = 0; i < 31; i++)
            flag[i] = flag[i+1];
        flag_value = atoi(flag);
        strcpy(flag, argv[2]);
        /* Searching for number from flag */
        while ((c = fgetc(f)) != EOF)
        {
            if ((flag[0] == '+') && (str_count == flag_value))
                position = ftell(f) - 1;
              while (c != '\n')
                c = fgetc(f);
              str_count += 1;
        }
        /* Found numbers of lines and n */
        rewind(f);
        delta = str_count - flag_value; /* For -n */
        if (flag[0] == '+')
        {
          fseek(f, position, SEEK_SET);
          while ((c = fgetc(f)) != EOF)
          {
              printf("%c", c);
          }
        }
        if ((argc = 3) && (flag[0] == '-'))
        {
            str = 1;
            while (((c = fgetc(f)) != EOF) && (str < delta))
            {
              while (c != '\n')
                  c = fgetc(f);
              str += 1;
            }
            printf("%c", c);
            while ((c = fgetc(f)) != EOF)
                printf("%c", c);
          }
    }

    if (argc == 2) /* No flags */
    {
        while ((c = fgetc(f)) != EOF)
        {
              while (c != '\n')
                c = fgetc(f);
              str_count += 1;
        }
        rewind(f);
        if (str_count <= 10)
        {
            while ((c = fgetc(f)) != EOF)
                printf("%c", c);
        }
        else
        {
            delta = str_count - 10;
            str = 1;
            while (((c = fgetc(f)) != EOF) && (str < delta))
            {
                while (c != '\n')
                    c = fgetc(f);
                str += 1;
            }
            printf("%c", c);
            while ((c = fgetc(f)) != EOF)
                printf("%c", c);
        }
    }
    fclose(f);
    return(1);
}
