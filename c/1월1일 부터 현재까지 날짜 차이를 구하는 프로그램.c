#include <stdio.h>

int getDays(int month, int day)
{
	int i, sum = 0;
	for(i = 1; i < month; i++)
	{
		if(1 == 2) // 2���� ������ ������,  ���α׷������� ������ �������� �ʽ��ϴ�. 
		{
			sum += 28;
		}
		else if(i % 2 == 0) // ¦�� ���� 30�� 
		{
			sum += 30;
		}
		else // Ȧ�� ���� 31�� 
		{
			sum += 31;	
		}
	}
	return sum + day; // sum -> Ư���� �ޱ����� ��¥ ������ ���ϴ� ��, day -> ��¥�� �״�� �����ִ� ��. 
 } 
 
int main(void)
{
	int month, day;
	printf("��¥�� �Է��ϼ��� : ");
	scanf("%d %d", &month, &day);
	printf("1�� 1�� ���� �ش� ��¥������ �Ÿ��� %d�� �Դϴ�.", getDays(month, day));
	return 0;
}
