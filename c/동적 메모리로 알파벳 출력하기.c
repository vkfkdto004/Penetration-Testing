#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *pc = NULL;
	int i = 0;
	pc = (char *)malloc(100 * sizeof(char)); // 총 100개의 문자가 들어갈 수 있음, 배열과 동일하다고 볼 수 있다. 
	if(pc == NULL)
	{
		printf("동적 메모리 할당에 실패했습니다.\n");
		exit(1);
	}
	// pc가 가르키는 포인터를 1씩 증가시키며 알파벳 소문자를 삽입한다. 
	for(i = 0; i < 26; i++)
	{
		*(pc + i) = 'a' + i;
	}
	
	/*
	*(pc) = pc[0]
	*(pc + 1) = pc[1]
	*(pc + 2) = pc[2]
	+1씩 되는 것이 할당되어있는 메모리의 다음번째를 의미하는데 이것은 배열에서 다음 순서를 가리키는 것과 동일하다. 
	*/
	*(pc + i) = 0; // 아스키 코드 0 -> NULL -> z 이후에 NULL 값이 들어가서 문자열이 종료된 것을 인식한다. 
	
	printf("%s\n", pc); 
	free(pc);
	return 0;
 } 
