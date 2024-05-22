#include <stdio.h>
#include <limits.h>

int main(void)
{
	int x = INT_MAX;
	printf("int형의 최댓값은 x는 %d입니다.\n", x);
	printf("x + 1은 %d입니다.", x + 1); 
	return 0;
}

//limits 는변수 또는 자료형의 한계값 범위들을 포함하고 있는 라이브러리이다. 
//overflow는 한계를 넘어서서 값이 초과되는 것을 의미한다. 
//한계점을 뛰어 넘으면 한바퀴를 건너뛰어 최소점으로 가버린다. 
