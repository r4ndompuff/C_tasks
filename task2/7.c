#include <stdio.h>
#include <setjmp.h>

jmp_buf begin;

char curlex;

void getlex(void);

int expr(void);

int add(void);

int exx(void);

int mult(void);

void error();

int abs(int n) {
    const int ret[2] = { n, -n };
    return ret [n<0];
}

void getlex() {
	while ((curlex = getchar()) == ' ');
}

void error(void) {
	printf("\nERROR!\n");
	while (getchar() != '\n');
	longjmp(begin, 1);
}

int expr() {
	int e = add();
		while (curlex == '+' || curlex == '-') {
			if (curlex == '+') {
				getlex();
				e += add();
			}
			else {
				getlex();
				e -= add();
			}
		}
	return e;
}

int add() {
	int a = exx();
	while (curlex == '*' || curlex == '/') {
		if (curlex == '*') {
			getlex();
			a *= exx();
		}
		else {
			getlex();
			a /= (int)(exx());
		}
	}
	return a;
}

int dirtywork(int a, int st) {
	if (st < 0)	{
		printf("\nERROR!\n");
		printf("But okay, we will absoulte your power!!!\n");
	}
	int res = a;
	st = abs(st);
	st = st - 1;
	if (st == -1)
		res = 1;
	while (st > 0) {
		res *= a;
		st -= 1;
	}
	return res;
}


int exx() {
	int d = mult();
	while (curlex == '^') {
		getlex();
		int step = exx();
		d = dirtywork(d, step);
	}
	return d;
}

int mult() {
	int m;
	switch(curlex) {
		case '0': case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9': m = curlex - '0'; break;
		case '(': getlex(); m = expr();
			if (curlex == ')') break;
		default : error();
	}
	getlex();
	return m;
}

int main() {
	int result;
	setjmp(begin);
	printf("==>");
	getlex();
	result = expr();
	if (curlex != '\n') error();
	printf("\nResult = %d\n", result);
	system("read");
}
















