#include <stdio.h>

int main(void)
{
	char o;
	int x, y;
	while(1)
	{
		printf("������ �Է��ϼ��� : ");
		scanf("%d %c %d", &x, &o, &y); 
		if(o == '+')
		{
			printf("%d %c %d = %d\n", x, o, y, x + y);
		}
		else if(o == '-')
		{
			printf("%d %c %d = %d\n", x, o, y, x - y);
		}
		else if(o == '*')
		{
			printf("%d %c %d = %d\n", x, o, y, x * y);
		}
		else if(o == '/')
		{
			printf("%d %c %d = %d\n", x, o, y, x / y);
		}
		else if(o == '%')
		{
			printf("%d %c %d = %d\n", x, o , y, x % y);
		}
		else
		{
			printf("�Է��� �߸��Ǿ����ϴ�.\n");
		}
		getchar(); //���� ó�����ؼ� ���͸� ȿ�������� ó��  
		printf("���α׷��� �����Ͻðڽ��ϱ�? (y/n) ");
		scanf("%c", &o);
		if (o == 'n' || o == 'N')
		{
			continue;
		}
		else if(o == 'y' || o == 'Y')
		{
			printf("\n���α׷��� �����մϴ�.");
			break;
		}
		else
		{
			printf("�Է��� �߸��Ǿ����ϴ�.\n");
		}
	} 
	return 0;
 } 
 /* 
 1. ���ۻ���(&) -> Ư����  ������ �޸� ��ü�� �ǹ��Ѵ�. 
 2. sacnf -> reads input && printf -> display output 
 */
