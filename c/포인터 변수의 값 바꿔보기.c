#include <stdio.h>

int main(void)
{
	int i = 10;
	int *p;
	p = &i;
	printf("i = %d\n", i); // i = 10
	*p = 20;
	printf("i = %d\n", i); // i = 20
	return 0;	
}
