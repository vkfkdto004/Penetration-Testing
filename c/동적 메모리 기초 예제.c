// ���� �޸� �Ҵ� -> ���α׷��� ���� ���߿� �������� �޸𸮸� �Ҵ� �޴� ���� ���Ѵ�. malloc() �迭�� ���̺귯�� �Լ��� ����Ͽ� �Ҵ� �޾� ��� ���� 
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *pi; // Pointer Integer
	pi = (int *)malloc(sizeof(int)); // malloc = �޸𸮸� �Ҵ��ض� ��� ��
	if(pi == NULL)
	{
		printf("���� �޸� �Ҵ翡 �����߽��ϴ�.\n");
		exit(1);
	} 
	*pi = 100;
	printf("%d\n", *pi);
	// ���� �޸𸮸� ����� ���Ŀ��� ������ �ش� �޸𸮸� ��ȯ�Ѵ�. �������� ȿ������ ���̱� ������
	free(pi); // free -> �޸� �Ҵ� ���� 
	return 0;
} 

