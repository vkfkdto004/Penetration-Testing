#include <stdio.h>
#include <string.h>

int main(void)
{
	char input[10] =  "I Love You";
	char result[5] = "Love";
	strcpy(result, input); // 내부적으로 포인터를 다루는 함수라고 할 수 있다. 
	printf("문자열 복사 : %s\n", result);
	return 0;
}
