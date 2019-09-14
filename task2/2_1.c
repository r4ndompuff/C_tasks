#include <stdio.h>

int res = 0;
int x, a;


int main() {
	printf("Type a polynomial: \n");
	scanf("%d", &x);
	while (scanf("%d", &a) != EOF) {
		res *= x;
		res += a;
	}
	printf("\nYour result is %i\n", res);
	system("read");
}