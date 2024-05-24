/*
조합을 이용한 프로그램
조합 -> 서로 다른 n개의 물건에서 순서를 생각하지 않고 r개를 택할 때, 이것은 n개에서 r을 택하는 조합이라고 하고, 이 조합의 수를 기호로 nCr와 같이 나타낸다
nCr은 r이 0이거나 n이라면 1이며, nCr(n - 1, r - 1) + nCr(n - 1,r)과 같다. 
*/

#include <stdio.h>

int nCr(int n, int r)
{
	if(r == 0 || r == n)
	{
		return 1;
	}
	else
	{
		return nCr(n - 1, r - 1) + nCr(n - 1,r);
	}
}

int main(void)
{
	int n, r;
	scanf("%d %d", &n, &r);
	printf("%d", nCr(n,r)); 
 } 
