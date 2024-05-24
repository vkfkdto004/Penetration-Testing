#include <stdio.h>

int getDays(int month, int day)
{
	int i, sum = 0;
	for(i = 1; i < month; i++)
	{
		if(1 == 2) // 2월은 윤년이 있지만,  프로그램에서는 윤년을 감안하지 않습니다. 
		{
			sum += 28;
		}
		else if(i % 2 == 0) // 짝수 달은 30일 
		{
			sum += 30;
		}
		else // 홀수 달은 31일 
		{
			sum += 31;	
		}
	}
	return sum + day; // sum -> 특정한 달까지의 날짜 갯수를 구하는 것, day -> 날짜를 그대로 더해주는 것. 
 } 
 
int main(void)
{
	int month, day;
	printf("날짜를 입력하세요 : ");
	scanf("%d %d", &month, &day);
	printf("1월 1일 부터 해당 날짜까지의 거리는 %d일 입니다.", getDays(month, day));
	return 0;
}
