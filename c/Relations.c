#include <stdio.h>

int main(void)
{
	int x = 50, y = 30;
	// x가 y와 같은 값을 가지고 있는가? 
	// 대수학에서는 x=y -> x와 y가 같은 상황 그 자체를 의미
	// x == y -> true, x와 y가 같은 값을 가지고 있는 것이다.
	printf("x가 y와 같은가 ? %d\n", x == y); 
	printf("x가 y와 다른가 ? %d\n", x != y);
	printf("x가 y보다 큰가 ? %d\n", x > y);
	printf("x가 y보다 작은가 ? %d\n", x < y);
	printf("x에 y값을  넣으면 ? %d\n", x = y);
	return 0;
}
