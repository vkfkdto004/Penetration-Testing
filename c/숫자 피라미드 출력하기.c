/*
반복 함수 :  단순히 while 또는 for 문을 이용하여 특정한 처리를 반복하는 방식으로 문제를 해결
재귀 함수 : 자신의 함수 내부에서 자기 자신을 스스로 호출함으로 써 재귀적으로 문제를 해결하는 것 
*/

//반복 함수를 이용해서 숫자 피라미드를 출력한다.
//반복 함수 같은 경우 오직 for문과 while 문으로 구성한다 
#include <stdio.h>

int print(int a)
{
	int i, j;
	for(i = 0; i < a; i++)
	{
		for(j = 0; j <= i; j++)
		{
			printf("%d ", j + 1);
		}
		printf("\n");
	}
} 

int main(void)
{
	int a;
	scanf("%d", &a);
	print(a);
	return 0;
}
