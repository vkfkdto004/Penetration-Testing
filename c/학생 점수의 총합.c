#include <stdio.h>

int main(void)
{
	int score[5][2];
	int total[2] = { 0, }; // ������ �迭�� �Ҵ��� �� ��� �����Ϳ� ���� 0�� �־��شٴ� ���̴�. 
	int i;
	
	for(i = 0; i < 5; i++)
	{
		printf("%d��° �л��� ����, ���� ���� : ", i + 1);
		scanf("%d %d", &score[i][0], &score[i][1]);
	} 
	for(i = 0; i < 5; i++)
	{
		total[0] += score[i][0];
		total[1] += score[i][1];
	}
	
	printf("\n\n5���� ���� ����\n �հ� : %d\n ��� : %d\n", total[0], (total[0]/5));
	printf("\n\n5���� ���� ����\n �հ� : %d\n ��� : %d\n", total[1], (total[1]/5));
	return 0;
}
