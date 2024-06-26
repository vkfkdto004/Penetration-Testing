#include <stdio.h>

int main(void)
{
	printf("Hello World!");
	return 0;
}

//hello world의 프로그램이며,
//#include <stdio.h> 이 부분은 라이브러리를 추가하는 부분이고,  하나의 기본 라이브러리이다. 
//c언어로 작성하면 바로 printf라는 함수를 사용할 수 있는 것은 아니고, 라이브러리를 추가해줘야 사용을 할 수 있다.
//printf는 사용자에게 입력값을 출력하는 가장 기본적인 함수이며,
//return 0;은 하나의 함수가 종료될 때 반환값을 정해주는 지정된 약속 같은 것이다.
//왜 해주는가?
//main 함수는 반환형이 int 이기 때문이다.
//void - 매개변수 : 특정한 함수를 실행할 때 함수가 처리할 어떤 값들을 받아오는 것을 의미한다.  
//소스코드 작성 -> 컴파일러가  목적 프로그램으로 변환 -> main 함수부터 실행되도록 약속이 되어있음. 
