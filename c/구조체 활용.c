#include <stdio.h>
#include <stdlib.h>

struct student {
	int number;
	char name[10];
	double grade;
};

int main(void)
{
	struct student s;
	
	printf("�й��� �Է����ּ��� : "); 
	scanf("%d", &s.number);
	printf("�̸��� �Է����ּ��� : "); 
	scanf("%s", s.name); // �迭�� �� ��ü�� �������� �ǹ̰� �־�, ���� �ּҸ� ����Ű�� &�� ��� �ȴ�. 
	printf("������ �Է����ּ��� : "); 
	scanf("%lf", &s.grade); // float -> %f , double -> %lf
	
	printf("�й� : %d\n", s.number);
	printf("�̸� : %s\n", s.name);
	printf("���� : %.1f\n", s.grade); 
	return 0;
}
