#include <stdio.h>
#include <math.h> // For fabsf() function

float E, res, x1, x0;
int x;


float square_root(float x0, int x, float E) {
	x1 = ((x0 + x/x0)/2);
    if (fabsf(x1 - x0) < E)
		return x1;
	else {
		x0 = x1;
		x1 = square_root(x0, x, E);
		return x1;
	}
}

int main() {
	printf("Type in Epsilon and your numbers: \n");
	scanf("%f", &E);
	while (scanf("%d", &x) != EOF) {
		res = square_root(1, x, E);
		printf("\nsqrt(%d) = %f", x, res);
	}
	system("read");
}