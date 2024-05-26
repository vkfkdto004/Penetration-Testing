#include <stdio.h>
#define SIZE 5 // 학생의 총 명수 

struct student {
	int number;
	char name[20];
	double grade; // GPA -> 학점 
};

int main(void)
{
	struct student list[SIZE];
	int i;
	
	for(i = 0; i < SIZE; i++)
	{
		printf("학번을 입력하세요 : ");
		scanf("%d", &list[i].number); 
		printf("이름을 입력하세요 : ");
		scanf("%s", &list[i].name);
		printf("학점을 입력하세요 : ");
		scanf("%lf", &list[i].grade); 
	}
	
	for(i = 0; i < SIZE; i++)
	{
	 	printf("학번 : %d, 이름 : %s, 학점 : %.1f\n", list[i].number, list[i].name, list[i].grade);
	}
	return 0;
} 
// 메모리에 일시적으로 학생들의 정보가 저장된다. 
