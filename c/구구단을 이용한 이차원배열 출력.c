// 다차원 배열 : 배열이 배열의 원소로 들어가는 구조 M X N 형태의 지도를 나타내고자 할 때 많이 사용된다. 
#include <stdio.h>

int main(void)
{
	int i, j;
	int gugudan[10][10];
	for(i = 1; i <= 9; i++)
	{
		printf("\n[ %d단 ]\n\n", i);
		for(j = 1; j <= 9; j++)
		{
			gugudan[i][j] = i * j;
			printf("%d X %d = %d\n", i, j ,gugudan[i][j]);
		}
	 } 
	return 0;
}
