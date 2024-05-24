//배열 -> 데이터가 많을 때 사용 (다양한 데이터를 삽입할 수 있는 공간) 
#include <stdio.h>
#include <limits.h>
#define NUMBER 5 

int main(void)
{
	int i, max, min, index;
	int array[NUMBER]; 
	max = 0;
	index = 0;
	/*
	i -> 임시적으로 사용하는  배열 각각의 위치
	max -> 최댓값
	min -> 최솟값
	index ->  순서 위치를 의미 
	*/
	for(i = 0; i < NUMBER; i++)
	{
		scanf("%d", &array[i]);
		if(max < array[i])
		{
			max = array[i];
			index = i;
		}
	}
	printf("가장 큰 값은 %d입니다. 그리고 %d번째에 있습니다.\n", max, index + 1);
	min = INT_MAX;
	for(i = 0; i < NUMBER; i++)
	{
		scanf("%d", &array[i]);
		if(min > array[i])
		{
			min = array[i];
			index = i;
		}
	}
	printf("가장 작은  값은 %d입니다. 그리고 %d번째에 있습니다.\n", min, index + 1);
	return 0;
 } 
