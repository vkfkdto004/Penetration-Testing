#include <stdio.h>
#include <math.h>

struct point {
	int x;
	int y;	
};

struct rect { // 사각형을 의미 
	struct point p1; // 가장 왼쪽 위에 있는 점 
	struct point p2; // 가장 오른쪽 아래에 있는 점 
};

int main(void)
{
	struct rect r; // r 이라는 사각형 하나 생성 
	int w, h, area, peri; // area 넓이 , peri 둘레
	
	printf("왼쪽 상단의 좌표를 입력하세요 : ");
	scanf("%d %d", &r.p1.x, &r.p1.y); 
	
	printf("오른쪽 하단의 좌표를 입력하세요 : ");
	scanf("%d %d", &r.p2.x, &r.p2.y);
	
	w = abs(r.p2.x - r.p1.x); // abs 절댓값 
	h = abs(r.p2.y - r.p1.y);
	
	area = w * h;
	peri = 2 * w + 2 * h;
	
	printf("사각형의 넓이는 %d이고, 둘레는 %d 입니다.\n", area, peri);
		
	return 0;	
}
