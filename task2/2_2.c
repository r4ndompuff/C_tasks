#include <stdio.h>

int res = 0;
int x, a, i;
int res1 = 0;
int r1 = 0;


int main() {
	printf("Type a polynomial: \n");
	scanf("%d", &x);
	i = 1;
	while (scanf("%d", &a) != EOF) {
		res *= x;
		res += a;
		// Dervativ
		res1 = res1 * x + r1;
		r1 = (r1 + a) * x;
	}
	printf("\nYour result is %i\n", res);
	printf("Your derivative is %i\n", (int)(res1/x));
	system("read");

}