#include <stdio.h>
#define MAX 10001

long  CMMDC(long a, long b)
{
	while (b != 0) {
		long r = a % b;
		a = b;
		b = r;
	}
	return a;
}

long CMMMC(long a, long b)
{
	return a / CMMDC(a, b) * b;
}

int main(void)
{
	int window;
	long perechi[MAX][2];
	scanf("%d", &window);
	int i = -1;
	do {
		i++;
		scanf("%ld %ld", &perechi[i][0], &perechi[i][1]);
	} while (perechi[i][0] && perechi[i][1]);
	int nr_perechi = i;
	for (i = 0; i < nr_perechi; i++) {
		for (int j = i + 1 ; j < nr_perechi &&
			 perechi[j][0] - perechi[i][0] <= window; j++) {
			long cmmdc = CMMDC(perechi[i][1], perechi[j][1]);
			long cmmmc = CMMMC(perechi[i][1], perechi[j][1]);
			printf("%ld %ld\n", cmmmc, cmmdc);
		}
	}
	return 0;
}
