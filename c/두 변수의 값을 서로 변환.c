//포인터 -> 특정 데이터가 저장된 메모리의 주소를 저장하는 변수 -> 다양한 연산을 수행할 수 있게 응용이 가능하고, 특정 주소를 그대로 이용하기 때문에 값이 즉시 변경된다는 특징이 있음. 
#include <stdio.h>

// 두 변수의 값을 서로 변환하는 포인터 함수 
void swap(int *x, int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
} 
int main(void)
{
	int x = 1;
	int y = 2;
	swap(&x, &y);
	printf("x = %d\ny = %d\n", x, y);
	return 0;
}
/*
포인터를 이용하지 않게 되면 매개변수로 넘오는 값들은 swap 함수 내에서만 처리되기 때문에 값이 변하지 않는다 -> main 함수에는 전혀 영향을 끼지치 못한다 
main 함수에서는 swap  함수의 x와 y를 사용만 했을 뿐 swap 함수를 통해서 x와 y 값이 변하지 않는다. 
*/
