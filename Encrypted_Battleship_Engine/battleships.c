//Vizdoaga Emilia-Ioana, 314 CD

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 101

int **calloc_tabla(int nl, int nc)
{
	int **tabla = calloc(nl + 1, sizeof(int *));
	if (!tabla) {
		printf("Eroare de alocare.\n");
		return NULL;
	}
	for (int i = 1; i <= nl; i++) {
		tabla[i] = calloc(nc + 1, sizeof(int));
		if (!tabla[i]) {
			printf("Eroare la alocarea liniei.\n");
			return NULL;
		}
	}
	return tabla;
}

int **calloc_coordonate(void)
{
	int **amplasare = calloc(MAX + 1, sizeof(int *));
	if (!amplasare) {
		printf("Eroare de alocare-amplasare.\n");
		return NULL;
	}
	for (int i = 1; i <= MAX; i++) {
		amplasare[i] = calloc(3, sizeof(int));
		if (!amplasare[i]) {
			printf("Eroare la alocarea-amplasare.\n");
			return NULL;
		}
	}
	return amplasare;
}

char **calloc_orientare(void)
{
	char **orientare = calloc(MAX + 1, sizeof(char *));
	if (!orientare) {
		printf("Eroare de alocare-amplasare.\n");
		return NULL;
	}
	for (int i = 1; i <= MAX; i++) {
		orientare[i] = calloc(3, sizeof(char));
		if (!orientare[i]) {
			printf("Eroare la alocarea-amplasare.\n");
			return NULL;
		}
	}
	return orientare;
}

float **calloc_date(int j)
{
	float **date = calloc(j + 1, sizeof(float *));
	if (!date) {
		printf("Eroare de alocare-acc.\n");
		return NULL;
	}
	for (int i = 1; i <= j; i++) {
		date[i] = calloc(5, sizeof(float));
		if (!date[i]) {
			printf("Eroare la alocarea-acc.\n");
			return NULL;
		}
	}
	return date;
}

int lungime_barca(char tip)
{
	int l;
	switch (tip) {
	case 'S':
		l = 5;
		break;
	case 'Y':
		l = 4;
		break;
	case 'B':
		l = 3;
		break;
	case 'L':
		l = 2;
		break;
	case 'A':
		l = 1;
		break;
	}
	return l;
}

void amplasare_gresita(char tip, char orientare, int x, int y)
{
	if (orientare == 'V') {
		switch (tip) {
		case 'S':
			printf("Nava Shinano nu poate fi amplasata");
			printf(" vertical la coordonatele (%d, %d).\n", x, y);
			break;
		case 'Y':
			printf("Nava Yamato nu poate fi amplasata");
			printf(" vertical la coordonatele (%d, %d).\n", x, y);
			break;
		case 'B':
			printf("Nava Belfast nu poate fi amplasata");
			printf(" vertical la coordonatele (%d, %d).\n", x, y);
			break;
		case 'L':
			printf("Nava Laffey nu poate fi amplasata");
			printf(" vertical la coordonatele (%d, %d).\n", x, y);
			break;
		case 'A':
			printf("Nava Albacore nu poate fi amplasata");
			printf(" vertical la coordonatele (%d, %d).\n", x, y);
			break;
		}
	} else {
		switch (tip) {
		case 'S':
			printf("Nava Shinano nu poate fi amplasata");
			printf(" orizontal la coordonatele (%d, %d).\n", x, y);
			break;
		case 'Y':
			printf("Nava Yamato nu poate fi amplasata");
			printf(" orizontal la coordonatele (%d, %d).\n", x, y);
			break;
		case 'B':
			printf("Nava Belfast nu poate fi amplasata");
			printf(" orizontal la coordonatele (%d, %d).\n", x, y);
			break;
		case 'L':
			printf("Nava Laffey nu poate fi amplasata");
			printf(" orizontal la coordonatele (%d, %d).\n", x, y);
			break;
		case 'A':
			printf("Nava Albacore nu poate fi amplasata");
			printf(" orizontal la coordonatele (%d, %d).\n", x, y);
			break;
		}
	}
}

int locuri_libere_vertical(int **tabla, int x, int y, int lungime)
{
	for (int i = x;  i > x - lungime; i--) {
		if (tabla[i][y] != 0) {
			return 0;
		}
	}
	return 1;
}

int locuri_libere_orizontal(int **tabla, int x, int y, int lungime)
{
	for (int i = y; i < y + lungime; i++) {
		if (tabla[x][i] != 0) {
			return 0;
		}
	}
	return 1;
}

int citeste_linii_amplasare(int **tabla, int nl, int nc, char ***orientare,
							int ***coordonate_amplasare_corect,
							int *amplasate_corect, int *alocat)
{
	if (*amplasate_corect % 99 == 0) {
		*alocat = *alocat + MAX;
		int **tmp1 = realloc(*coordonate_amplasare_corect,
			(*alocat + 1) * sizeof(int *));
		if (tmp1) {
			for (int i = *alocat - MAX + 1; i <=  *alocat; i++) {
				tmp1[i] = calloc(3, sizeof(int));
			}
			*coordonate_amplasare_corect = tmp1;
		} else {
			printf("Eroare la realocare.\n");
		}
		char **tmp2 = realloc(*orientare, (*alocat + 1) * sizeof(char *));
		if (tmp2) {
			for (int i = *alocat - MAX + 1; i <= *alocat; i++) {
				tmp2[i] = calloc(2, sizeof(char));
			}
			*orientare = tmp2;
		} else {
			printf("Eroare la realocare.\n");
		}
	}
	char tip, orient;
	int x, y;
	scanf(" %c %c %d %d", &tip, &orient, &x, &y);
	int lungime = lungime_barca(tip);
	if (x > nl || y > nc || (orient == 'V' && lungime > x) ||
		(orient == 'H' && lungime > nc - y + 1)) {
		amplasare_gresita(tip, orient, x, y);
		return 0;
	}
	if (orient == 'V') {
		if (locuri_libere_vertical(tabla, x, y, lungime) == 0) {
			amplasare_gresita(tip, orient, x, y);
			return 0;
		}
		for (int i = x;  i > x - lungime; i--) {
			tabla[i][y] = lungime;
		}
	} else {
		if (locuri_libere_orizontal(tabla, x, y, lungime) == 0) {
			amplasare_gresita(tip, orient, x, y);
			return 0;
		}
		for (int i = y; i < y + lungime; i++) {
			tabla[x][i] = lungime;
		}
	}
	(*amplasate_corect)++;
	(*coordonate_amplasare_corect)[*amplasate_corect][1] = x;
	(*coordonate_amplasare_corect)[*amplasate_corect][2] = y;
	(*orientare)[*amplasate_corect][0] = orient;
	return 1;
}

void afisare_table(int **tabla1, int **tabla2, int nl, int nc)
{
	for (int i = 1; i <= nl; i++) {
		for (int j = 1; j <= nc; j++) {
			printf("%d ", tabla1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 1; i <= nl; i++) {
		for (int j = 1; j <= nc; j++) {
			printf("%d ", tabla2[i][j]);
		}
		printf("\n");
	}
}

char *nume_nava(int lungime)
{
	switch (lungime) {
	case 5:
		return "Shinano";
	case 4:
		return "Yamato";
	case 3:
		return "Belfast";
	case 2:
		return "Laffey";
	case 1:
		return "Albacore";
	}
	return 0;
}

int atac_jucator1(int **tabla2, int **coordonate_aplasare_corect,
				  char **orientare, int ship_m, int x, int y,
				  int *amplasate_corect, int *nimerite1)
{
	int ship = ship_m;
	int lungime = tabla2[x][y];
	switch (tabla2[x][y]) {
	case 0:
		printf("Jucatorul 1 a ratat o lovitura");
		printf(" la coordonatele (%d, %d).\n", x, y);
		tabla2[x][y] = -1;
		return ship;
	case -1:
		printf("Coordonatele (%d, %d) au fost", x, y);
		printf(" deja atacate de jucatorul 1.\n");
		return ship;
	}
	(*nimerite1)++;
	char *nume = nume_nava(tabla2[x][y]);
	for (int i = 2; i <= *amplasate_corect; i = i + 2) {
		if (coordonate_aplasare_corect[i][1] == x &&
			coordonate_aplasare_corect[i][2] == y) {
			if (orientare[i][0] == 'H') {
				printf("Jucatorul 1 a distrus o nava %s", nume);
				printf(" plasata intre coordonatele");
				printf(" (%d, %d) si (%d, %d).\n", x, y, x, y + lungime - 1);
				for (int i = y; i < y + lungime; i++) {
					if (tabla2[x][i] > 0) {
						tabla2[x][i] = 0;
					}
				}
			} else {
				printf("Jucatorul 1 a distrus o nava %s plasata intre", nume);
				printf(" coordonatele (%d, %d) si", x, y);
				printf(" (%d, %d).\n", x - lungime + 1, y);
				for (int i = x; i > x - lungime; i--) {
					if (tabla2[i][y] > 0) {
						tabla2[i][y] = 0;
					}
				}
			}
			tabla2[x][y] = -1;
			return --ship;
		}
	}
	tabla2[x][y] = -1;
	printf("Jucatorul 1 a lovit o nava %s la", nume);
	printf(" coordonatele (%d, %d).\n", x, y);
	return ship;
}

int atac_jucator2(int **tabla1, int **coordonate_aplasare_corect,
				  char **orientare, int ship_g, int x, int y,
				  int *amplasate_corect, int *nimerite2)
{
	int ship = ship_g;
	int lungime = tabla1[x][y];
	switch (tabla1[x][y]) {
	case 0:
		printf("Jucatorul 2 a ratat o lovitura la");
		printf(" coordonatele (%d, %d).\n", x, y);
		tabla1[x][y] = -1;
		return ship;
	case -1:
		printf("Coordonatele (%d, %d) au ", x, y);
		printf("fost deja atacate de jucatorul 2.\n");
		return ship;
	}
	(*nimerite2)++;
	char *nume = nume_nava(tabla1[x][y]);
	for (int i = 1; i <= *amplasate_corect; i = i + 2) {
		if (coordonate_aplasare_corect[i][1] == x &&
			coordonate_aplasare_corect[i][2] == y) {
			if (orientare[i][0] == 'H') {
				printf("Jucatorul 2 a distrus o nava %s", nume);
				printf(" plasata intre coordonatele ");
				printf("(%d, %d) si (%d, %d).\n", x, y, x, y + lungime - 1);
				for (int i = y; i < y + lungime; i++) {
					if (tabla1[x][i] > 0) {
						tabla1[x][i] = 0;
					}
				}
			} else {
				printf("Jucatorul 2 a distrus o nava %s", nume);
				printf(" plasata intre coordonatele ");
				printf("(%d, %d) si (%d, %d).\n", x, y, x - lungime + 1, y);
				for (int i = x; i > x - lungime; i--) {
					if (tabla1[i][y] > 0) {
						tabla1[i][y] = 0;
					}
				}
			}
			tabla1[x][y] = -1;
			return --ship;
		}
	}
	tabla1[x][y] = -1;
	printf("Jucatorul 2 a lovit o nava %s", nume);
	printf(" la coordonatele (%d, %d).\n", x, y);
	return ship;
}

void atacuri(int **tabla1, int **tabla2, int *amplasate_corect,
			 char **orientare, int **coordonate_amplasare_corect, int *nr_nave,
			 int *nimerite1, int *nimerite2, int *totale1, int *totale2)
{
	int ship_g = *nr_nave, ship_m = *nr_nave, cnt = 1;
	while (ship_g != 0 && ship_m != 0) {
		int x, y;
		scanf(" %d %d", &x, &y);
		if (cnt % 2 == 1) {
			ship_m = atac_jucator1(tabla2, coordonate_amplasare_corect,
								   orientare, ship_m, x, y, amplasate_corect,
								   nimerite1);
			cnt++; (*totale1)++;
			if (ship_m == 0) {
				printf("Jucatorul 1 a castigat.\n");
				break;
			}
		} else {
			ship_g = atac_jucator2(tabla1, coordonate_amplasare_corect,
								   orientare, ship_g, x, y, amplasate_corect,
								   nimerite2);
			if (ship_g == 0) {
				printf("Jucatorul 2 a castigat.\n");
			}
			cnt++; (*totale2)++;
		}
	}
}

void free_int(int **matrice, int nl)
{
	for (int i = 1; i <= nl; i++) {
		if (matrice[i]) {
			free(matrice[i]);
			matrice[i] = NULL;
		}
	}
	if (matrice) {
		free(matrice);
		matrice = NULL;
	}
}

void free_char(char **matrice, int nl)
{
	for (int i = 1; i <= nl; i++) {
		if (matrice[i]) {
			free(matrice[i]);
			matrice[i] = NULL;
		}
	}
	if (matrice) {
		free(matrice);
		matrice = NULL;
	}
}

void free_date(float **date, int j)
{
	for (int i = 1; i <= j; i++) {
		if (date[i]) {
			free(date[i]);
			date[i] = NULL;
		}
	}
	if (date) {
		free(date);
		date = NULL;
	}
}

float numar_nerotunjit(float x)
{
	int y = x * 100.0;
	x = (float)y / 100.0;
	return x;
}

int cerinta_1(float **date, float acc_dorita, float suma_totala,
			  int j, int nave_total)
{
	float maxi, acc_totala = suma_totala / (float)nave_total;
	int k, meciuri = 0;
	if (acc_totala >= acc_dorita) {
		return 0;
	}
	while (acc_totala < acc_dorita) {
		maxi = (100 - date[1][4]) * date[1][3], k = 1;
		for (int i = 2; i <= j; i++) {
			if (maxi < (100 - date[i][4]) * date[i][3]) {
				maxi = (100 - date[i][4]) * date[i][3];
				k = i;
			}
		}
		suma_totala += maxi;
		acc_totala = suma_totala / (float)nave_total;
		meciuri++;
		date[k][4] = 101.0;
	}
	return meciuri;
}

int cerinta_3(float **date, float acc_dorita, float suma_totala,
			  int j, int nave_total)
{
	float acc_totala = suma_totala / (float)nave_total, mini;
	int k, meciuri = 0;
	if (acc_totala < acc_dorita) {
		return 0;
	}
	while (acc_totala >= acc_dorita) {
		mini = date[1][4] * date[1][3];
		k = 1;
		for (int i = 1; i <= j; i++) {
			if (date[i][4] * date[i][3] < mini && date[i][4] != 101) {
				mini = date[i][4] * date[i][3];
				k = i;
			}
		}
		suma_totala -= mini;
		acc_totala = suma_totala / (float)nave_total;
		meciuri++;
		date[k][4] = 101.0;
	}
	return meciuri - 1;
}

void afisare_jucator(char *citire, float **date, int j)
{
	int cerinta1, cerinta2, cerinta3, cerinta4;
	int meci = (citire[2] - '0') * 100 + (citire[3] - '0') * 10
				+ (citire[4] - '0');
	float acc_dorita = (citire[5] - '0') * 100 + (citire[6] - '0') * 10 +
					   (citire[7] - '0') + (citire[9] - '0') * 0.1 +
					   (citire[10] - '0') * 0.01;
	if (citire[1] == 'U') {
		if (meci > j) {
			printf("Nu exista date pentru meciul cerut.\n");
		} else {
			float acc_meci = numar_nerotunjit((float)date[meci][1] /
							date[meci][2] * 100);
			printf("%06.2f.", acc_meci);
			if (citire[12] == 'I') {
				if (acc_meci >= acc_dorita) {
					cerinta2 = 0;
			} else {
				float lov_mini = date[meci][1] + 1;
				while (numar_nerotunjit((float)lov_mini / date[meci][2] * 100)
					   < acc_dorita) {
					lov_mini++;
				}
				cerinta2 = lov_mini - date[meci][1];
			}
			printf("%d\n", cerinta2);
			} else {
				float lov_max = date[meci][1] - 1;
				while (lov_max / date[meci][2] * 100 >= acc_dorita) {
					lov_max--;
				}
				cerinta4 = date[meci][1] - lov_max - 1;
				printf("%d\n", cerinta4);
			}
		}
	} else {
		float suma_totala = 0, nave_total = 0;
		for (int i = 1; i <= j; i++) {
			date[i][4] = (float)date[i][1] / date[i][2] * 100.0;
			suma_totala += date[i][4] * date[i][3];
			nave_total += date[i][3];
		}
		float acc_totala = numar_nerotunjit(suma_totala / nave_total);
		printf("%06.2f.", acc_totala);
		if (citire[12] == 'I') {
			cerinta1 = cerinta_1(date, acc_dorita, suma_totala, j, nave_total);
			printf("%d\n", cerinta1);
		} else if (citire[12] == 'A') {
			cerinta3 = cerinta_3(date, acc_dorita, suma_totala, j, nave_total);
			printf("%d\n", cerinta3);
		}
	}
}

int **calloc_cheie(void)
{
	int **cheie = calloc(4, sizeof(int *));
	if (!cheie) {
		printf("Eroare de alocare-cheie.\n");
		return NULL;
	}
	for (int i = 1; i <= 3; i++) {
		cheie[i] = calloc(4, sizeof(int));
		if (!cheie[i]) {
			printf("Eroare la alocarea-cheie.\n");
			return NULL;
		}
	}
	return cheie;
}

int decodificare(char element)
{
	if (element == '.') {
		return 36;
	}
	if (element >= '0' && element <= '9') {
		return element - '0';
	}
	return 10 + element - 'A';
}

char codificare(int element)
{
	if (element == 36) {
		return '.';
	}
	if (element >= 0 && element <= 9) {
		return element + '0';
	}
	return 'A' - 10 + element;
}

void free_cheie(int **cheie)
{
	for (int i = 1; i <= 3; i++) {
		if (cheie[i]) {
			free(cheie[i]);
			cheie[i] = NULL;
		}
	}
	if (cheie) {
		free(cheie);
		cheie = NULL;
	}
}

int **calloc_bloc(void)
{
	int **bloc = calloc(4, sizeof(int *));
	if (!bloc) {
		printf("Eroare de alocare-bloc.\n");
		return NULL;
	}
	for (int i = 1; i <= 3; i++) {
		bloc[i] = calloc(2, sizeof(int));
		if (!bloc[i]) {
			printf("Eroare la alocarea-bloc.\n");
			return NULL;
		}
	}
	return bloc;
}

void free_bloc(int **bloc)
{
	for (int i = 1; i <= 3; i++) {
		if (bloc[i]) {
			free(bloc[i]);
			bloc[i] = NULL;
		}
	}
	if (bloc) {
		free(bloc);
		bloc = NULL;
	}
}

int determinant_2(int **matrice_key)
{
	int det_key = matrice_key[1][1] * matrice_key[2][2];
	det_key -= matrice_key[1][2] * matrice_key[2][1];
	return det_key;
}

int determinant_3(int **matrice_key)
{
	int det_key = matrice_key[1][1] * matrice_key[2][2] * matrice_key[3][3];
	det_key += matrice_key[2][1] * matrice_key[3][2] * matrice_key[1][3];
	det_key += matrice_key[3][1] * matrice_key[1][2] * matrice_key[2][3];
	det_key -= matrice_key[1][3] * matrice_key[2][2] * matrice_key[3][1];
	det_key -= matrice_key[2][3] * matrice_key[3][2] * matrice_key[1][1];
	det_key -= matrice_key[3][3] * matrice_key[1][2] * matrice_key[2][1];
	return det_key;
}

int **adjuncta2(int **matrice_key2, int **adjuncta)
{
	adjuncta[1][1] = matrice_key2[2][2];
	adjuncta[2][2] = matrice_key2[1][1];
	adjuncta[1][2] = -matrice_key2[1][2];
	adjuncta[2][1] = -matrice_key2[2][1];
	return adjuncta;
}

int **adjuncta3(int **matrice_key, int **adjuncta)
{
	int c11 = matrice_key[2][2] * matrice_key[3][3] -
			 matrice_key[2][3] * matrice_key[3][2];
	int c12 = -(matrice_key[2][1] * matrice_key[3][3] -
			 matrice_key[2][3] * matrice_key[3][1]);
	int c13 = matrice_key[2][1] * matrice_key[3][2] -
			 matrice_key[2][2] * matrice_key[3][1];
	int c21 = -(matrice_key[1][2] * matrice_key[3][3] -
			 matrice_key[1][3] * matrice_key[3][2]);
	int c22 = matrice_key[1][1] * matrice_key[3][3] -
			 matrice_key[1][3] * matrice_key[3][1];
	int c23 = -(matrice_key[1][1] * matrice_key[3][2] -
			 matrice_key[1][2] * matrice_key[3][1]);
	int c31 = matrice_key[1][2] * matrice_key[2][3] -
			 matrice_key[1][3] * matrice_key[2][2];
	int c32 = -(matrice_key[1][1] * matrice_key[2][3] -
			 matrice_key[1][3] * matrice_key[2][1]);
	int c33 = matrice_key[1][1] * matrice_key[2][2] -
			 matrice_key[1][2] * matrice_key[2][1];
	adjuncta[1][1] = c11; adjuncta[2][1] = c12; adjuncta[3][1] = c13;
	adjuncta[1][2] = c21; adjuncta[2][2] = c22; adjuncta[3][2] = c23;
	adjuncta[1][3] = c31; adjuncta[2][3] = c32; adjuncta[3][3] = c33;
	return adjuncta;
}

int invers_multiplicativ(int det, int K)
{
	for (int x = 1; x < K; x++) {
		if ((det * x) % K == 1) {
			return x;
		}
	}
	return -1;
}

void criptare(int **matrice_key, char *afisare)
{
	size_t len = strlen(afisare);
	size_t cnt = 0;
	while (cnt < len) {
		int k = 0;
		int **bloc = calloc_bloc();
		if (!bloc) {
			return;
		}
		while (k < 3 && cnt < len) {
			bloc[k + 1][1] = decodificare(afisare[cnt++]);
			k++;
		}
		for (int i = 1; i <= k; i++) {
			long long suma = 0;
			for (int j = 1; j <= k; j++) {
				suma += (long long)matrice_key[i][j] * bloc[j][1];
			}
			if (suma < 0) {
				suma = suma % 37 + 37;
			} else {
				suma = suma % 37;
			}
			afisare[cnt - k + i - 1] = codificare(suma);
		}
		free_bloc(bloc);
	}
	printf("%s\n", afisare);
}

void acc_afisare(char *afisare, float acc)
{
	int acc1 = (int)(acc + 0.5f);
	afisare[0] = acc1 / 10000 + '0';
	afisare[1] = acc1 % 10000 / 1000 + '0';
	afisare[2] = acc1 % 1000 / 100 + '0';
	afisare[3] = '.';
	afisare[4] = acc1 % 100 / 10 + '0';
	afisare[5] = acc1 % 10 + '0';
	afisare[6] = '\0';
}

void cerinta_afisare(char *afisare, int cerinta)
{
	char temp[4];
	sprintf(temp, "%03d", cerinta);
	strcat(afisare, temp);

}

void calculare_statistici(int **matrice_key, char *citire, float **date, int j)
{
	int cerinta1, cerinta2, cerinta3, cerinta4;
	char *afisare = calloc(15, sizeof(char));
	if (!afisare) {
		printf("Eroare la alocare-afisare.\n");
	}
	int meci = (citire[2] - '0') * 100 + (citire[3] - '0') * 10
				+ (citire[4] - '0');
	float acc_dorita = (citire[5] - '0') * 100 + (citire[6] - '0') * 10 +
					   (citire[7] - '0') + (citire[9] - '0') * 0.1 +
					   (citire[10] - '0') * 0.01;
	if (citire[1] == 'U') {
		if (meci > j) {
			printf("Nu exista date pentru meciul cerut.\n");
		} else {
			float acc_meci = numar_nerotunjit((float)date[meci][1] /
							date[meci][2] * 100);
			acc_afisare(afisare, acc_meci * 100);
			if (citire[12] == 'I') {
				if (acc_meci >= acc_dorita) {
					cerinta2 = 0;
				} else {
					float lov_mini = date[meci][1] + 1;
					while (numar_nerotunjit((float)lov_mini / date[meci][2]
						   * 100) < acc_dorita) {
						lov_mini++;
					}
					cerinta2 = lov_mini - date[meci][1];
				}
				cerinta_afisare(afisare, cerinta2);
				criptare(matrice_key, afisare);
			} else {
				float lov_max = date[meci][1] - 1;
				while (lov_max / date[meci][2] * 100 >= acc_dorita) {
					lov_max--;
				}
				cerinta4 = date[meci][1] - lov_max - 1;
				cerinta_afisare(afisare, cerinta4);
				criptare(matrice_key, afisare);
			}
		}
	} else {
		float suma_totala = 0, nave_total = 0;
		for (int i = 1; i <= j; i++) {
			date[i][4] = (float)date[i][1] / date[i][2] * 100.0;
			suma_totala += date[i][4] * date[i][3];
			nave_total += date[i][3];
		}
		float acc_totala = numar_nerotunjit(suma_totala / nave_total);
		acc_afisare(afisare, acc_totala * 100);
		if (citire[12] == 'I') {
			cerinta1 = cerinta_1(date, acc_dorita, suma_totala, j, nave_total);
			cerinta_afisare(afisare, cerinta1);
			criptare(matrice_key, afisare);
		} else if (citire[12] == 'A') {
			cerinta3 = cerinta_3(date, acc_dorita, suma_totala, j, nave_total);
			cerinta_afisare(afisare, cerinta3);
			criptare(matrice_key, afisare);
		}
	}
}

void decriptare(int **matrice_key, char *mesaj, float **date_g,
				float **date_m, int j)
{
	size_t len = strlen(mesaj);
	size_t cnt = 0;
	while (cnt < len) {
		int k = 0, det;
		int **bloc = calloc_bloc(), **inmultire = calloc_bloc();
		int **matrice_key2 = calloc_cheie(), **adjuncta = calloc_cheie();
		if (!bloc) {
			return;
		}
		while (k < 3 && cnt < len) {
			if (mesaj[cnt] == '\n' || mesaj[cnt] == '\0') {
				break;
			}
			bloc[k + 1][1] = decodificare(mesaj[cnt++]);
			k++;
		}
		if (k == 0) {
			free_cheie(matrice_key2);
			free_bloc(bloc); free_bloc(inmultire); free_cheie(adjuncta);
			cnt++;
			continue;
		}
		if (k == 1) {
			det = matrice_key[1][1];
			adjuncta[1][1] = 1;
		} else if (k == 2) {
			matrice_key2[1][1] = matrice_key[1][1];
			matrice_key2[1][2] = matrice_key[1][2];
			matrice_key2[2][1] = matrice_key[1][3];
			matrice_key2[2][2] = matrice_key[2][1];
			det = determinant_2(matrice_key2);
			adjuncta = adjuncta2(matrice_key2, adjuncta);
		} else {
			det = determinant_3(matrice_key);
			adjuncta = adjuncta3(matrice_key, adjuncta);
		}
		if (det >= 0) {
			det = det % 37;
		} else {
			det = det % 37 + 37;
		}
		int x = invers_multiplicativ(det, 37);
		for (int i = 1; i <= k; i++) {
			for (int j = 1; j <= k; j++) {
				if (adjuncta[i][j] >= 0) {
					adjuncta[i][j] = adjuncta[i][j] % 37 * x;
				} else {
					adjuncta[i][j] = (adjuncta[i][j] % 37 + 37) * x;
				}
			}
		}
		for (int i = 1; i <= k; i++) {
			int suma = 0;
			for (int j = 1; j <= k; j++) {
				suma += adjuncta[i][j] * bloc[j][1];
			}
			if (suma < 0) {
				suma = suma % 37 + 37;
			} else {
				suma = suma % 37;
			}
			inmultire[i][1] = suma;
			mesaj[cnt - k + i - 1] = codificare(inmultire[i][1]);
		}
		free_cheie(matrice_key2);
		free_bloc(bloc), free_bloc(inmultire), free_cheie(adjuncta);
	}
	if (mesaj[0] == 'O') {
		calculare_statistici(matrice_key, mesaj, date_g, j);
	} else if (mesaj[0] == 'T') {
		calculare_statistici(matrice_key, mesaj, date_m, j);
	}
}

void statistici(float **date_g, float **date_m, int j)
{	scanf(" ");
	char *key = calloc(15, sizeof(char));
	if (!key) {
		printf("Eroare la alocare citire.\n");
	}
	fgets(key, 15, stdin);
	if (key[0] == '-') {
		char *citire = calloc(20, sizeof(char));
		if (!citire) {
			printf("Eroare la alocare citire.\n");
		}
		fgets(citire, 20, stdin);
		if (citire[0] != 'Q') {
			do {
				if (citire[0] == 'O') {
					afisare_jucator(citire, date_g, j);
				} else if (citire[0] == 'T') {
					afisare_jucator(citire, date_m, j);
				}
				fgets(citire, 20, stdin);
			} while (citire[0] != 'Q');
		}
		free(citire);
	} else if (key[0] != 'Q') {
		int **matrice_key = calloc_cheie(), cnt = 0;
		for (int i = 1; i <= 3; i++) {
			for (int j = 1; j <= 3; j++) {
				matrice_key[i][j] = decodificare(key[cnt++]);
			}
		}
		char *mesaj = calloc(20, sizeof(char));
		if (!mesaj) {
			printf("Eroare la alocare-mesaj.\n");
		}
		fgets(mesaj, 20, stdin);
		if (mesaj[0] != 'Q') {
			do {
				decriptare(matrice_key, mesaj, date_g, date_m, j);
				fgets(mesaj, 15, stdin);
			} while (mesaj[0] != 'Q');
		}
		free(mesaj);
		free_cheie(matrice_key);
	}
	free(key);
}

int main(void)
{
	int j = 0, aux = 0;
	scanf("%d ", &j);
	float **date_g = calloc_date(j), **date_m = calloc_date(j);
	while (aux++ < j) {
		int n, m, amplasate_corect = 0, k = 1, alocat = MAX;
		int nimerite_g = 0, nimerite_m = 0, total_g = 0, total_m = 0;
		scanf("%d %d", &n, &m);
		int nr_nave = n * m / 70 + n * m / 55 + n * m / 40 +
					 n * m / 30 + n * m / 20;
		int **tabla_g = calloc_tabla(n, m), **tabla_m = calloc_tabla(n, m);
		int **coordonate_amplasare_corect = calloc_coordonate();
		char **orientare = calloc_orientare();
		while (k++ <= nr_nave) {
			int ok1 = 0;
			while (ok1 == 0) {
				ok1 = citeste_linii_amplasare(tabla_g, n, m, &orientare,
											  &coordonate_amplasare_corect,
											  &amplasate_corect, &alocat);
			}
			int ok2 = 0;
			while (ok2 == 0) {
				ok2 = citeste_linii_amplasare(tabla_m, n, m, &orientare,
											  &coordonate_amplasare_corect,
											  &amplasate_corect, &alocat);
			}
		}
		afisare_table(tabla_g, tabla_m, n, m);
		atacuri(tabla_g, tabla_m, &amplasate_corect, orientare,
				coordonate_amplasare_corect, &nr_nave,
				&nimerite_g, &nimerite_m, &total_g, &total_m);
		date_g[aux][1] = nimerite_g; date_g[aux][2] = total_g;
		date_m[aux][1] = nimerite_m; date_m[aux][2] = total_m;
		date_g[aux][3] = nr_nave; date_m[aux][3] = nr_nave;
		free_int(tabla_g, n); free_int(tabla_m, n);
		free_char(orientare, alocat);
		free_int(coordonate_amplasare_corect, alocat);
	}
	statistici(date_g, date_m, j);
	free_date(date_g, j); free_date(date_m, j);
	return 0;
}
