#include <stdio.h>

char str[20];

double str2double(char str[]) {
	int bldot = 0;
	int blexp = 0;
	int blexpneg = 0;
	int blexppos = 0;
	double db = 0;
	int i = 0;
	int k = 10;
	int st;
	while (str[i] != '\0') {
		if (str[i] == 'e' || str[i] == 'E'){
			blexp = 1;
		}
		else if (blexp == 1) {
			if (str[i] == '-') {
				blexpneg = 1;
			}
			else {
				blexppos = 1;
			}
			blexp = 0;
		}
		else if (blexpneg == 1) {
			st = (int)str[i] - (int)('0');
			while (st > 0) {
				db /= 10;
				st -= 1;
			}
		}
		else if (blexppos == 1) {
			st = (int)str[i] - (int)('0');
			while (st > 0) {
				db *= 10;
				st -= 1;
			}
		}
		else if (bldot == 1) {
			db = db + ((float)str[i] - (float)('0'))/k;
			k *= 10;
		}
		else if (str[i] == '.') {
			bldot = 1;
		}
		else if (str[i] != '.') {
			db = db * 10 + ((int)str[i] - (int)('0'));
		}
		i += 1;
	}
	return db;
}

int main() {
	while (scanf("%s", str) != EOF)
		printf("%s = %.10g\n", str, str2double(str));
	system('read');
}
