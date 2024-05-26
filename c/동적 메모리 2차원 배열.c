#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i, x, y;
	int** pptr = (int**)malloc(sizeof(int*) * 8);
	for(i = 0; i < 8; i++)
	{
		*(pptr + i) = (int *)malloc(sizeof(int) * 6);
	}
	
	for(y = 0; y < 8; y++)
	{
		for(x = 0; x <  6; x++)
		{
			*(*(pptr + y) + x) = 6 * y + x;
		}
	}
	
	for(y = 0; y < 8; y++)
	{
		for(x = 0; x < 6; x++)
		{
			printf("%3d", *(*(pptr + y) + x));
		}
		printf("\n");
	}
	
	for(y = 0; y < 8; y++)
	{
		free(*(pptr + y));
	}
	return 0;
}
