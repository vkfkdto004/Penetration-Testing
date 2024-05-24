//어셈블리어 : 어셈블리어는 기계어와 1:1 대응되는 컴퓨터 프로그래밍의 저급 언어이다.
//포인터 : 특정한 데이터가 저장되어있는 메모리의 주소 그 자체를 저장하는 변수
//사용자 정의 함수 : 정해진 특정한 기능을 수행하는 모듈을 의미 -> 메인 함수의 크기를 줄여주고 각각의 거대한 프로그램을 운영하는 기능을 잘게 분해해서 나중에 하나로 합치는 것에 용이하다. 
#include <stdio.h>

// 전역 변수 : 프로그램 전체에서 어디서든지 접근 가능한 변수
int hour;
int minute;
int minuteAdd;

void counter() //void -> 함수의 반환 값이 없을 때 사용한다. (return이 없어도 된다) 
{
	minute += minuteAdd;
	hour += minute / 60;
	minute %= 60;
	hour %= 24;
}
 
int main(void)
{
	printf("시를 입력해주세요 : ");
	scanf("%d", &hour); 
	printf("분을 입력해주세요 : ");
	scanf("%d", &minute);
	printf("더할 분을 입력해주세요 : ");
	scanf("%d", &minuteAdd);
	counter(); 
	printf("더해진 시간은 %d시 %d분 입니다.\n", hour, minute);
	return 0;
}
