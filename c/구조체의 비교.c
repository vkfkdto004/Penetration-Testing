#include <stdio.h>

struct point {
	int x;
	int y; 
};

void comparePoint (struct point p1, struct point p2)
{
	if((p1.x == p2.x) && (p1.y == p2.y)) 
	{
		printf("p1과 p2는 같습니다. ");
	}
	// if문이 main 함수에 있는 것보다 함수를 이용하면 더 체계적으로 보일 수 있다. 
}

int main(void)
{
	struct point p1;
	struct point p2;
	
	p1.x = 30;
	p1.y = 10;
	
	p2.x = 30;
	p2.y = 10;
	
	/*
	if(p1 == p2)
	{
		printf("p1과 p2는 같습니다. ");
	}
	Error 발생 ->  구조체 자체는 그대로 비교를 못한다. 구조체 별로 특징을 잡아서 비교를 해줘야한다. 
	*/
	
	comparePoint(p1, p2);
	return 0;
}
