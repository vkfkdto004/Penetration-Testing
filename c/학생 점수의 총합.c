#include <stdio.h>

int main(void)
{
	int score[5][2];
	int total[2] = { 0, }; // 일차원 배열을 할당할 때 모든 데이터에 값을 0을 넣어준다는 뜻이다. 
	int i;
	
	for(i = 0; i < 5; i++)
	{
		printf("%d번째 학생의 수학, 영어 점수 : ", i + 1);
		scanf("%d %d", &score[i][0], &score[i][1]);
	} 
	for(i = 0; i < 5; i++)
	{
		total[0] += score[i][0];
		total[1] += score[i][1];
	}
	
	printf("\n\n5명의 수학 점수\n 합계 : %d\n 평균 : %d\n", total[0], (total[0]/5));
	printf("\n\n5명의 영어 점수\n 합계 : %d\n 평균 : %d\n", total[1], (total[1]/5));
	return 0;
}
