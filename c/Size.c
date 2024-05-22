#include <stdio.h>

int main(void)
{
	int x = 5;
	float y = 123456789.12345;
	double z =  123456789.12345;
	printf("x = %d\n", x);
	printf("y = %.2f\n", y);
	printf("z = %.2f\n", z);
	printf("변수 x의 메모리 크기는 %d입니다.\n", sizeof(x));
	printf("변수 y의 메모리 크기는 %d입니다.\n", sizeof(y));
	printf("변수 z의 메모리 크기는 %d입니다.", sizeof(z));
	return 0;
}

//%d - int(정수) 
//sizeof() - 기본 함수중 하나, 변수의 메모리 크기를 알려준다. 
//자료형이 어떤 변수던 간에 int면 4 byte
//int - 4 byte
//float - 4 byte
//double - 8 byte
