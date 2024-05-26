// 동적 메모리 할당 -> 프로그램이 실행 도중에 동적으로 메모리를 할당 받는 것을 말한다. malloc() 계열의 라이브러리 함수를 사용하여 할당 받아 사용 가능 
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *pi; // Pointer Integer
	pi = (int *)malloc(sizeof(int)); // malloc = 메모리를 할당해라 라는 뜻
	if(pi == NULL)
	{
		printf("동적 메모리 할당에 실패했습니다.\n");
		exit(1);
	} 
	*pi = 100;
	printf("%d\n", *pi);
	// 동적 메모리를 사용한 이후에는 무조건 해당 메모리를 반환한다. 안정성과 효율성을 높이기 위함임
	free(pi); // free -> 메모리 할당 해제 
	return 0;
} 

