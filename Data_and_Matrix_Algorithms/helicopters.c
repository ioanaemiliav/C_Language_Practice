#include <stdio.h>
#define MAX 1001

void schimbare(int *r1, int *c1, int *r2, int *c2)
{
	int aux1 = *r1;
	*r1 = *r2;
	*r2 = aux1;
	int aux2 = *c1;
	*c1 = *c2;
	*c2 = aux2;
}

int zone_afectate(int r1, int c1, int r2, int c2, int s, int a[MAX][MAX])
{
	int afectate = 0;
	//partea stanga-sus
	if (s == 1 && r2 > r1 && c2 < c1) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == 1 && r1 > r2 && c1 < c2) {
		for (int i = r2; i <= r1; i++) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 0) {
					afectate++;
				}
			}
			c2--;
		}
		return afectate;
	}
		//partea dreapta-sus
	if (s == 1 && r1 > r2 && c1 > c2) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == 1 && r1 < r2 && c1 < c2) {
		for (int i = r1; i <= r2; i++) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 0) {
					afectate++;
				}
			}
			c1++;
		}
		return afectate;
	}
	//partea stanga-jos
	if (s == -1 && r1 > r2 && c1 > c2) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == -1 && r1 < r2 && c1 < c2) {
		for (int i = r2; i >= r1; i--) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 0) {
					afectate++;
				}
			}
			c2--;
		}
		return afectate;
	}
		//partea dreapta-jos
	if (s == -1 && r1 > r2 && c2 > c1) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == -1 && r1 < r2 && c2 < c1) {
		for (int i = r2; i >= r1; i--) {
			for (int j = c2; j <= c1; j++) {
				if (a[i][j] == 0) {
					afectate++;
				}
			}
			c2++;
		}
		return afectate;
	}
	return afectate;
}

int zone_neafectate(int r1, int c1, int r2, int c2, int s, int a[MAX][MAX])
{
	int neafectate = 0;
	//partea stanga-sus
	if (s == 1 && r2 > r1 && c2 < c1) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == 1 && r1 > r2 && c1 < c2) {
		for (int i = r2; i <= r1; i++) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 1) {
					neafectate++;
				}
			}
			c2--;
		}
		return neafectate;
	}
		//partea dreapta-sus
	if (s == 1 && r1 > r2 && c1 > c2) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == 1 && r1 < r2 && c1 < c2) {
		for (int i = r1; i <= r2; i++) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 1) {
					neafectate++;
				}
			}
			c1++;
		}
		return neafectate;
	}
	//partea stanga-jos
	if (s == -1 && r1 > r2 && c1 > c2) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == -1 && r1 < r2 && c1 < c2) {
		for (int i = r2; i >= r1; i--) {
			for (int j = c1; j <= c2; j++) {
				if (a[i][j] == 1) {
					neafectate++;
				}
			}
			c2--;
		}
		return neafectate;
	}
		//partea dreapta-jos
	if (s == -1 && r1 > r2 && c2 > c1) {
		schimbare(&r1, &c1, &r2, &c2);
	}
	if (s == -1 && r1 < r2 && c2 < c1) {
		for (int i = r2; i >= r1; i--) {
			for (int j = c2; j <= c1; j++) {
				if (a[i][j] == 1) {
					neafectate++;
				}
			}
			c2++;
		}
		return neafectate;
	}
	return neafectate;
}

int main(void)
{
	int n, m, a[MAX][MAX], k, v[800][5], x = 0, y = 0, gresit[800];
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			scanf("%d", &a[i][j]);
		}
	}
	scanf("%d", &k);
	for (int i = 1; i <= k; i++) {
		scanf("%d %d %d %d %d", &v[i][1], &v[i][2],
			  &v[i][3], &v[i][4], &v[i][5]);
	}
	for (int i = 1; i <= k; i++) {
		if (v[i][1] == v[i][3] || v[i][2] == v[i][4]) {
			printf("Elicopterul %d este pozitionat necorespunzator!\n", i);
		} else {
			int afectate = zone_afectate(v[i][1], v[i][2], v[i][3], v[i][4],
				v[i][5], a);
			int neafectate = zone_neafectate(v[i][1], v[i][2], v[i][3], v[i][4],
				v[i][5], a);
			if (afectate == 0) {
				x++;
			}
			if (afectate > neafectate) {
				gresit[++y] = i;
			}
		}
	}
	printf("%d\n", x);
	printf("%d\n", y);
	for (int i = 1; i <= y; i++) {
		printf("%d ", gresit[i]);
	}
	printf("\n");
	return 0;
}
