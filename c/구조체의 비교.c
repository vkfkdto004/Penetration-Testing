#include <stdio.h>

struct point {
	int x;
	int y; 
};

void comparePoint (struct point p1, struct point p2)
{
	if((p1.x == p2.x) && (p1.y == p2.y)) 
	{
		printf("p1�� p2�� �����ϴ�. ");
	}
	// if���� main �Լ��� �ִ� �ͺ��� �Լ��� �̿��ϸ� �� ü�������� ���� �� �ִ�. 
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
		printf("p1�� p2�� �����ϴ�. ");
	}
	Error �߻� ->  ����ü ��ü�� �״�� �񱳸� ���Ѵ�. ����ü ���� Ư¡�� ��Ƽ� �񱳸� ������Ѵ�. 
	*/
	
	comparePoint(p1, p2);
	return 0;
}
