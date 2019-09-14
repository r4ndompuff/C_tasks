#include <stdio.h>
#define M 63

char alph[M] = {"0123456789abcdefghijklmnoprqstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "};
char c, c1, c2;
int i, j, min;


int main() {
	// Insert_sort
	for (i = 0; i < M; i++) {
		min = i;
		for (j = i + 1; j < M; j++)
			if ((int)alph[j] < (int)alph[min]) min = j;
		j = alph[i];
		alph[i] = alph[min];
		alph[min] = j;
	}

	// Output
	for (i = 0; i < M; ++i) {
		if (alph[i] == ' ')
			printf("Space = %d\n", ' ');
		else
		printf("%c = %d\n", alph[i], (int)alph[i]);
	}

	printf("\n");

	// Difference_calc
	printf("Write any 2 symbols (without SPACE): \n");
	scanf("%c", &c1);
	scanf("%c", &c2);
	printf("\n");
	printf("The difference between 2 symbols is %d\n", (int)c1-(int)c2);

	system("read");
}