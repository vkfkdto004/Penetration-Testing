// 구조체 -> 객체 지향 프로그래밍에서 클래스의 모체가 되는 것으로 여러 개의 자료형을 묶어서 새로운 자료형을 만들 수 있는 방법 
#include <stdio.h>
#include <stdlib.h>

struct student {
	int number;
	char name[10];
	double grade;
};

int main(void)
{
	struct student s;
	s.number = 20150001;
	strcpy(s.name, "홍길동");
	s.grade = 4.5;
	printf("학번 : %d\n", s.number);
	printf("이름 : %s\n", s.name);
	printf("학점 : %.1f\n", s.grade); 
	return 0;
}
