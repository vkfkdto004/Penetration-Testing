#include <stdio.h>
#include <limits.h>

int main(void)
{
	int x = INT_MAX;
	printf("int���� �ִ��� x�� %d�Դϴ�.\n", x);
	printf("x + 1�� %d�Դϴ�.", x + 1); 
	return 0;
}

//limits �º��� �Ǵ� �ڷ����� �Ѱ谪 �������� �����ϰ� �ִ� ���̺귯���̴�. 
//overflow�� �Ѱ踦 �Ѿ�� ���� �ʰ��Ǵ� ���� �ǹ��Ѵ�. 
//�Ѱ����� �پ� ������ �ѹ����� �ǳʶپ� �ּ������� ��������. 
