#include <stdio.h>

int main(void)
{
	int x = 5;
	float y = 123456789.12345;
	double z =  123456789.12345;
	printf("x = %d\n", x);
	printf("y = %.2f\n", y);
	printf("z = %.2f\n", z);
	printf("���� x�� �޸� ũ��� %d�Դϴ�.\n", sizeof(x));
	printf("���� y�� �޸� ũ��� %d�Դϴ�.\n", sizeof(y));
	printf("���� z�� �޸� ũ��� %d�Դϴ�.", sizeof(z));
	return 0;
}

//%d - int(����) 
//sizeof() - �⺻ �Լ��� �ϳ�, ������ �޸� ũ�⸦ �˷��ش�. 
//�ڷ����� � ������ ���� int�� 4 byte
//int - 4 byte
//float - 4 byte
//double - 8 byte
