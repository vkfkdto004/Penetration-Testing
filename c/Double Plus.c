#include <stdio.h>

int main(void)
{
	int x = 0;
	printf("현재의 x는 %d입니다.\n", x);
	x++;
	printf("현재의 x는 %d입니다.\n", x);
	printf("현재의 x는 %d입니다.\n", x--);
	printf("현재의 x는 %d입니다.\n", x);
	printf("현재의 x는 %d입니다.\n", --x);
	return 0;
}
