// 문자열 -> 다양한 문자 표현을 나타내기 위해 사용, 내부적으로 문자열은 char 자료 타입의 배열로 이루어져 있으며 이것은 포인터와 같다고 할 수 있다. 
// char -> 1byte, 영어 1개크기 == 1byte, 한글 1개의 크기 == 2byte 
#include <stdio.h>

int main(void)
{
	char input[1001];
	gets(input);
	int count = 0;
	while(input[count] != '\0') // \0 == null
	{
		count++;
	}
	printf("입력한 문자열의 길이는 %d입니다.\n", count);
	printf("입력한 문자열 : %s", input);
	return 0;
}
