//�迭 -> �����Ͱ� ���� �� ��� (�پ��� �����͸� ������ �� �ִ� ����) 
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
	i -> �ӽ������� ����ϴ�  �迭 ������ ��ġ
	max -> �ִ�
	min -> �ּڰ�
	index ->  ���� ��ġ�� �ǹ� 
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
	printf("���� ū ���� %d�Դϴ�. �׸��� %d��°�� �ֽ��ϴ�.\n", max, index + 1);
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
	printf("���� ����  ���� %d�Դϴ�. �׸��� %d��°�� �ֽ��ϴ�.\n", min, index + 1);
	return 0;
 } 
