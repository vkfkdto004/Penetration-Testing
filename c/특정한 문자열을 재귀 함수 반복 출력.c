#include <stdio.h>

// 함수 안에서 자기 자신을 포함하는 형태 -> 재귀 함수(Recursive Function) 
void print(int count)
{
	if(count == 0)
	{
		return;
	}
	else
	{
		printf("문자열을 출력합니다.\n");
		print(count - 1);
	}
 } 
int main(void)
{
	int number;
	printf("문자열을 몇 개 출력할까요?");
	scanf("%d", &number); 
	print(number);
	return 0;
}
