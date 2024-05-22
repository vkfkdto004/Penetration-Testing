#include <stdio.h>

int main(void)
{
	char x = 65;
	printf("%c\n", x);
	char y = 'A';
	printf("%d\n", y);
	char z = 'B';
	printf("%d\n", z);
	return 0;
}

//char - 하나의 문자를 담는 공간 
//%c - char형을 출력해줄 수 있는 약속 
//char 형은 내부적으로 숫자로 구성된다.  A의 아스키코드는 65, x변수 값에 65를 넣어도 동일한 결과가 나온다. 
