#include <stdio.h>

int main() {
	printf("Integer = %lu%s\n", sizeof(int), " bytes");
	printf("Short = %lu%s\n", sizeof(short), " bytes");
	printf("Long = %lu%s\n", sizeof(long), " bytes");
	printf("Float = %lu%s\n", sizeof(float), " bytes");
	printf("Double = %lu%s\n", sizeof(double), " bytes");
	printf("Long Double = %lu%s\n", sizeof(long double), " bytes");	
	system("read"); 
}
