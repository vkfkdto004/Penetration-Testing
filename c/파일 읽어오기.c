#include <stdio.h>

int main(void)
{
	FILE *fp = NULL;
	int c;
	
	fp = fopen("input.txt", "r");
	
	if(fp == NULL)
	{
		printf("���� �б⿡ �����߽��ϴ�.\n");
	} 
	else
	{
		printf("���� ���⿡ �����߽��ϴ�.\n"); 
	} 
	
	while((c = fgetc(fp)) != EOF)
	{
		putchar(c); 
	}
	
	fclose(fp);
	return 0;
}
