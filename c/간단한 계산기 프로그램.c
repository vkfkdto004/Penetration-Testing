#include <stdio.h>

int main(void)
{
	char o;
	int x, y;
	while(1)
	{
		printf("수식을 입력하세요 : ");
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
			printf("입력이 잘못되었습니다.\n");
		}
		getchar(); //버퍼 처리를해서 엔터를 효율적으로 처리  
		printf("프로그램을 종료하시겠습니까? (y/n) ");
		scanf("%c", &o);
		if (o == 'n' || o == 'N')
		{
			continue;
		}
		else if(o == 'y' || o == 'Y')
		{
			printf("\n프로그램을 종료합니다.");
			break;
		}
		else
		{
			printf("입력이 잘못되었습니다.\n");
		}
	} 
	return 0;
 } 
 /* 
 1. 엠퍼샌드(&) -> 특정한  변수에 메모리 자체를 의미한다. 
 2. sacnf -> reads input && printf -> display output 
 */
