#include <stdio.h>

// �Լ� �ȿ��� �ڱ� �ڽ��� �����ϴ� ���� -> ��� �Լ�(Recursive Function) 
void print(int count)
{
	if(count == 0)
	{
		return;
	}
	else
	{
		printf("���ڿ��� ����մϴ�.\n");
		print(count - 1);
	}
 } 
int main(void)
{
	int number;
	printf("���ڿ��� �� �� ����ұ��?");
	scanf("%d", &number); 
	print(number);
	return 0;
}
