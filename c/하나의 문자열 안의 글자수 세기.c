// ���ڿ� -> �پ��� ���� ǥ���� ��Ÿ���� ���� ���, ���������� ���ڿ��� char �ڷ� Ÿ���� �迭�� �̷���� ������ �̰��� �����Ϳ� ���ٰ� �� �� �ִ�. 
// char -> 1byte, ���� 1��ũ�� == 1byte, �ѱ� 1���� ũ�� == 2byte 
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
	printf("�Է��� ���ڿ��� ���̴� %d�Դϴ�.\n", count);
	printf("�Է��� ���ڿ� : %s", input);
	return 0;
}
