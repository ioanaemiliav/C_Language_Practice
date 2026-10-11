#include <stdio.h>
#define MAX 50

/* Propagate light rays ('x') horizontally and vertically from each bulb ('L') */
void propagare_lumina(int a, int b, char v[MAX][MAX])
{
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			if (v[i][j] == 'L') {
				for (int k = j + 1; k < b; k++) {
					if (v[i][k] == '#' ||
						(v[i][k] >= 48 && v[i][k] <= 52)) {
						break;
					}
					v[i][k] = 'x';
				}
				for (int k = j - 1; k >= 0; k--) {
					if (v[i][k] == '#' ||
						(v[i][k] >= 48 && v[i][k] <= 52)) {
						break;
					}
					v[i][k] = 'x';
				}
				for (int k = i + 1; k < a; k++) {
					if (v[k][j] == '#' ||
						(v[k][j] >= 48 && v[k][j] <= 52)) {
						break;
					}
					v[k][j] = 'x';
				}
				for (int k = i - 1; k >= 0; k--) {
					if (v[k][j] == '#' ||
						(v[k][j] >= 48 && v[k][j] <= 52)) {
						break;
					}
					v[k][j] = 'x';
				}
			}
		}
	}
}

/* Block remaining empty spaces around a numbered cell if its bulb capacity is already met */
void modif_blocuri_adiacente(int a, int b, char v[MAX][MAX])
{
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			if (v[i][j] >= '0' && v[i][j] <= '4') {
				int nr_becuri = v[i][j] - '0';
				int becuri_adiacente = 0;
				if (v[i][j + 1] == 'L') {
					becuri_adiacente++;
				}
				if (v[i + 1][j] == 'L') {
					becuri_adiacente++;
				}
				if (v[i - 1][j] == 'L') {
					becuri_adiacente++;
				}
				if (v[i][j - 1] == 'L') {
					becuri_adiacente++;
				}
				if (nr_becuri == becuri_adiacente) {
					if (v[i][j + 1] == '-') {
						v[i][j + 1] = 'x';
					}
					if (v[i + 1][j] == '-') {
						v[i + 1][j] = 'x';
					}
					if (v[i - 1][j] == '-') {
						v[i - 1][j] = 'x';
					}
					if (v[i][j - 1] == '-') {
						v[i][j - 1] = 'x';
					}
				}
			}
		}
	}
}

/* Validate that no two bulbs illuminate each other directly */
int luminare_reciproca(int a, int b, char v[MAX][MAX])
{
	int ok = 1;
	for (int i = 0; i < a && (ok != 0); i++) {
		for (int j = 0; j < b  && (ok != 0); j++) {
			if (v[i][j] == 'L') {
				for (int k = j + 1; k < b && ok != 0; k++) {
					if (v[i][k] == '#' || (v[i][k] >= 48 && v[i][k] <= 52)) {
						break;
					} else if (v[i][k] == 'L') {
						ok = 0;
					}
				}
				for (int k = i + 1; k < a && ok != 0; k++) {
					if (v[k][j] == '#' || (v[k][j] >= 48 && v[k][j] <= 52)) {
						break;
					} else if (v[k][j] == 'L') {
						ok = 0;
					}
				}
			}
		}
	}
	return ok;
}

/* Ensure no numbered block is surrounded by more bulbs than its maximum limit */
int becuri_adiacente(int a, int b, char v[MAX][MAX])
{
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b ; j++) {
			if (v[i][j] >= 48 && v[i][j] <= 52) {
				int nr_becuri = 0;
				if (j < b - 1 && v[i][j + 1] == 'L') {
					nr_becuri++;
				}
				if (i < a - 1 && v[i + 1][j] == 'L') {
					nr_becuri++;
				}
				if (i > 0 && v[i - 1][j] == 'L') {
					nr_becuri++;
				}
				if (j > 0 && v[i][j - 1] == 'L') {
					nr_becuri++;
				}
				if (nr_becuri > (v[i][j] - '0')) {
					return 0;
				}
			}
		}
	}
	return 1;
}

/* Verify if a numbered block still has enough empty adjacent spaces to reach its required bulb count */
int lumineaza_celula(int a, int b, char v[MAX][MAX])
{	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b ; j++) {
			if (v[i][j] >= 48 && v[i][j] <= 52) {
				int locuri_posibile = 0;
				int nr_becuri = 0;
				if (j < b - 1 && v[i][j + 1] == '-') {
					locuri_posibile++;
				} else if (j < b - 1 && v[i][j + 1] == 'L') {
					nr_becuri++;
				}
				if (i < a - 1 && v[i + 1][j] == '-') {
					locuri_posibile++;
				} else if (i < a - 1 && v[i + 1][j] == 'L') {
					nr_becuri++;
				}
				if (i > 0 && v[i - 1][j] == '-') {
					locuri_posibile++;
				} else if (i > 0 && v[i - 1][j] == 'L') {
					nr_becuri++;
				}
				if (j > 0 && v[i][j - 1] == '-') {
					locuri_posibile++;
				} else if (j > 0 && v[i][j - 1] == 'L') {
					nr_becuri++;
				}
				if (locuri_posibile + nr_becuri < (v[i][j] - '0')) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int main(void)
{
	int o, n, m;
	char grid[MAX][MAX];
	scanf("%d %d %d", &o, &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf(" %c", &grid[i][j]);
		}
	}
	
	if (o == 1) {
		propagare_lumina(n, m, grid);
		modif_blocuri_adiacente(n, m, grid);
	 //afisare grid modificat
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				printf("%c ", grid[i][j]);
			}
			printf("\n");
		}
	}
	
	if (o == 2) {
		if (luminare_reciproca(n, m, grid) == 0) {
			printf("zero\n");
			return 0;
		}
		if (becuri_adiacente(n, m, grid) == 0) {
			printf("zero\n");
			return 0;
		}
		propagare_lumina(n, m, grid);
		if (lumineaza_celula(n, m, grid) == 0) {
			printf("zero\n");
			return 0;
		}
		printf("ichi\n");
	}
	return 0;
}
