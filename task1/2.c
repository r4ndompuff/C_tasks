#include <stdio.h>

unsigned char uc = -1;
signed char sc = -1;
char ch = -1;

int main() {
	int i = uc;
	int k = sc;
	int j = ch;
	printf("Unsigned = %d\n", i);
	printf("Signed = %d\n", k);
	printf("Char = %d\n", j);
	printf("\n");
	if (j == k)
		printf("Your system is using signed type for char.\n");
	else if (j == i)
		printf("Your system is using unsigned type for char.\n");
	else
		printf("I don't know what happened with your PC, please, fix it.\n");
	system("read");
}