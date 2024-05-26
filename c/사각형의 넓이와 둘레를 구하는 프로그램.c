#include <stdio.h>
#include <math.h>

struct point {
	int x;
	int y;	
};

struct rect { // �簢���� �ǹ� 
	struct point p1; // ���� ���� ���� �ִ� �� 
	struct point p2; // ���� ������ �Ʒ��� �ִ� �� 
};

int main(void)
{
	struct rect r; // r �̶�� �簢�� �ϳ� ���� 
	int w, h, area, peri; // area ���� , peri �ѷ�
	
	printf("���� ����� ��ǥ�� �Է��ϼ��� : ");
	scanf("%d %d", &r.p1.x, &r.p1.y); 
	
	printf("������ �ϴ��� ��ǥ�� �Է��ϼ��� : ");
	scanf("%d %d", &r.p2.x, &r.p2.y);
	
	w = abs(r.p2.x - r.p1.x); // abs ���� 
	h = abs(r.p2.y - r.p1.y);
	
	area = w * h;
	peri = 2 * w + 2 * h;
	
	printf("�簢���� ���̴� %d�̰�, �ѷ��� %d �Դϴ�.\n", area, peri);
		
	return 0;	
}
