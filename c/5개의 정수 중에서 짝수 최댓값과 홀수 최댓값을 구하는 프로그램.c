#include  <stdio.h>
#define NUMBER 5
 
int main(void)
{
	int array[NUMBER];
	int i, oddMax, evenMax;
	oddMax = 0;
	evenMax = 0;
	for(i = 0; i < NUMBER; i++)
	{
		scanf("%d", &array[i]);
		if(array[i] % 2 == 0)
		{
			if(evenMax < array[i])
			{
				evenMax = array[i];
			}
		}
		else
		{
			if(oddMax < array[i])
			{
				oddMax = array[i];	
			}
		}
	}
	printf("Ȧ�� �ִ��� %d �Դϴ�.\n", oddMax);
	printf("¦�� �ִ��� %d �Դϴ�.\n", evenMax);
	return 0;
}
