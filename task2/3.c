#include <stdio.h>

int i, itr, rek;
int r1, r2, r;

int rek_fib(int r1, int r2, int i) {
	if (i <= 0)
		return r1;
	else {
		r = r1 + r2;
		r1 = r2;
		i -= 1;
		r2 = rek_fib(r1, r, i);
		return r2;
	}
}

int itr_fib(int i) {
	int j = 1;
	r1 = 0;
	r2 = 1;
	for (j = 1; j <= i; j++) {
		r = r1 + r2;
		r1 = r2;
		r2 = r;
	}
	return r1;
}

int main(){
	r = 0;
	printf("Type any numbers: \n");
	while (scanf("%d", &i) != EOF) {
		rek = rek_fib(0, 1, i);
		itr = itr_fib(i);
		printf("Fibonacci(%d)=Iterationaly(%d)=Recursive(%d)\n", i, itr, rek);
	}
	system("read");
}