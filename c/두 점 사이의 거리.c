#include <stdio.h>
#include <math.h>

struct point{
	int x;
	int y;
};

int main(void)
{
	struct point p1, p2;
	int xDiff, yDiff;
	double distance;
	
	printf("1�� ���� ��ǥ�� �Է��ϼ��� : ");
	scanf("%d %d", &p1.x, &p1.y);
	getchar(); 
	printf("2�� ���� ��ǥ�� �Է��ϼ��� : ");
	scanf("%d %d", &p2.x, &p2.y);
	
	xDiff = p1.x - p2.x;
	yDiff = p1.y - p2.y;
	
	distance = sqrt(xDiff * xDiff + yDiff * yDiff);
	printf("�� �� ������ �Ÿ��� %f�Դϴ�.\n", distance);
	return 0;
}
