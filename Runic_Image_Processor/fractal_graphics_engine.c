#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX 5000
#define PI 3.1415926

typedef struct {
	double x, y, orientare;
} stare_testoasa;

typedef struct {
	int cod;
	int dwx;
	int dwy;
	int bbw;
	int bbh;
	int bbxoff;
	int bbyoff;
	unsigned char **bitmaps;
} chars;

typedef struct {
	char *fisier;
	char *axioma;
	int nrules;
	char *simbol;
	char **succesor;
	int w;
	int h;
	unsigned char **pixeli;
	int nglyph;
	char *nume_font;
	chars *caracter;
} comanda; // salvez comenzile care pot fi folosite de undo/redo

/* Safely reallocate memory for the turtle's spatial state stack */
stare_testoasa *realocare_stare(stare_testoasa *stare, int cnt)
{
	stare_testoasa *tmp = realloc(stare, cnt * 2 * sizeof(stare_testoasa));
	if (!tmp) {
		printf("Eroare la realocarea stazrii!\n");
		free(stare);
		return NULL;
	}
	stare = tmp;
	return stare;
}

/* Safely reallocate memory for the command history stack (Undo/Redo) */
comanda *realocare_comanda(comanda *comanda_, int cnt)
{
	comanda *tmp = realloc(comanda_, cnt * 2 * sizeof(comanda));
	if (!tmp) {
		printf("Eroare la realocarea stazrii!\n");
		free(comanda_);
		return NULL;
	}
	comanda_ = tmp;
	return comanda_;
}

/* Allocate dynamic memory for string buffers */
char *alocare_dinamica(void)
{
	char *sir = malloc(MAX * sizeof(char));
	if (!sir) {
		printf("Eroare de alocare.\n");
		return NULL;
	}
	sir[0] = '\0';
	return sir;
}

/* Safely reallocate memory for dynamic string scaling */
char *realocare_dinamica(char *sir, int cnt)
{
	char *tmp1 = realloc(sir, cnt * 2 * sizeof(char));
	if (tmp1) {
		sir = tmp1;
	} else {
		printf("Eroare la realocare.\n");
	}
	return sir;
}

/* Deallocate a string pointer and set to NULL */
void free_sir(char **sir)
{
	if (*sir) {
		free(*sir);
		*sir = NULL;
	}
}

/* Deallocate a 2D character array */
void free_tablou(char **tablou, int cnt)
{
	if (tablou) {
		for (int i = 0; i < cnt; i++) {
			free(tablou[i]);
		}
		free(tablou);
	}
}

/* Deallocate a 2D unsigned character array (e.g., pixel matrices) */
void free_tablou_unsigned(unsigned char **tablou, int cnt)
{
	if (tablou) {
		for (int i = 0; i < cnt; i++) {
			free(tablou[i]);
		}
		free(tablou);
	}
}

/* Free all resources allocated in the Undo history stack */
void free_undo(comanda *comanda_undo, int cnt_undo)
{
	for (int i = 0; i <= cnt_undo; i++) {
		free_sir(&comanda_undo[i].fisier);
		free_sir(&comanda_undo[i].axioma);
		free_sir(&comanda_undo[i].simbol);
		free_tablou(comanda_undo[i].succesor, comanda_undo[i].nrules);
		free_tablou_unsigned(comanda_undo[i].pixeli, comanda_undo[i].h);
		free_sir(&comanda_undo[i].nume_font);
		if (comanda_undo[i].caracter) {
			for (int j = 0; j < comanda_undo[i].nglyph; j++) {
				free_tablou_unsigned(comanda_undo[i].caracter[j].bitmaps,
									 comanda_undo[i].caracter[j].bbh);
			}
			free(comanda_undo[i].caracter);
		}
	}
}

/* Free all resources allocated in the Redo history stack */
void free_redo(comanda *comanda_redo, int cnt_redo)
{
	for (int i = 0; i <= cnt_redo; i++) {
		free_sir(&comanda_redo[i].fisier);
		free_sir(&comanda_redo[i].axioma);
		free_sir(&comanda_redo[i].simbol);
		free_tablou(comanda_redo[i].succesor, comanda_redo[i].nrules);
		free_tablou_unsigned(comanda_redo[i].pixeli, comanda_redo[i].h);
		free_sir(&comanda_redo[i].nume_font);
		if (comanda_redo[i].caracter) {
			for (int j = 0; j < comanda_redo[i].nglyph; j++) {
				free_tablou_unsigned(comanda_redo[i].caracter[j].bitmaps,
									 comanda_redo[i].caracter[j].bbh);
			}
			free(comanda_redo[i].caracter);
		}
	}
}

/* Revert the last undoable command and push it to the Redo stack */
void undo(comanda *comanda_undo, comanda *comanda_redo,
		  int *cnt_undo, int *cnt_redo)
{
	if (*cnt_undo == -1) {
		printf("Nothing to undo\n");
	} else {
		(*cnt_redo)++;
		comanda_redo[*cnt_redo] = comanda_undo[*cnt_undo];
		(*cnt_undo)--;
	}
}

/* Re-apply the last reverted command and push it back to the Undo stack */
void redo(comanda *comanda_undo, comanda *comanda_redo,
		  int *cnt_undo, int *cnt_redo)
{
	if (*cnt_redo == -1) {
		printf("Nothing to redo\n");
	} else {
		(*cnt_undo)++;
		comanda_undo[*cnt_undo] = comanda_redo[*cnt_redo];
		if (comanda_undo[*cnt_undo].axioma) { // Command is L-system
			printf("Loaded %s (", comanda_undo[*cnt_undo].fisier);
			printf("L-system with %d rules)\n", comanda_undo[*cnt_undo].nrules);
		}
		// Command is Load
		if (comanda_undo[*cnt_undo].fisier && comanda_undo[*cnt_undo].pixeli) {
			printf("Loaded %s (", comanda_undo[*cnt_undo].fisier);
			printf("PPM image %dx%d)\n", comanda_undo[*cnt_undo].w,
				   comanda_undo[*cnt_undo].h);
		} else if (comanda_undo[*cnt_undo].pixeli &&
				   !comanda_undo[*cnt_undo].nglyph) { // Command is Turtle
			printf("Drawing done\n");
		}
		if (comanda_undo[*cnt_undo].nume_font) { // Command is Font
			printf("Loaded %s (", comanda_undo[*cnt_undo].fisier);
			printf("bitmap font %s)\n", comanda_undo[*cnt_undo].nume_font);
		} else if (comanda_undo[*cnt_undo].pixeli &&
				   comanda_undo[*cnt_undo].nglyph) { // Command is Type
			printf("Text written\n");
		}
		(*cnt_redo)--;
	}
}

/* Parse and load a Lindenmayer System (L-System) configuration from a file */
void lsystem(comanda *comanda_undo, comanda  *comanda_redo,
			 int *cnt_undo, int *cnt_redo, char *buff)
{
	char *fisier = alocare_dinamica();
	strcpy(fisier, buff + 8);
	fisier[strlen(fisier) - 1] = '\0';
	FILE *date = fopen(fisier, "rt");
	if (!date) {
		printf("Failed to load %s\n", fisier);
		free_sir(&fisier);
		return;
	}
	char *axioma = alocare_dinamica();
	int nrules = 0;
	fscanf(date, "%s", axioma);
	fscanf(date, "%d", &nrules);
	char *simbol = malloc(nrules * sizeof(char));
	if (!simbol) {
		printf("Eroare de alocare.\n");
	}
	char **succesor = malloc(nrules * sizeof(char *));
	if (!succesor) {
		printf("Eroare de alocare.\n");
	}
	for (int i = 0; i < nrules; i++) {
		succesor[i] = malloc(MAX * sizeof(char));
		if (!succesor[i]) {
			printf("Eroare la alocarea liniei.\n");
		} else {
			succesor[i][0] = '\0';
		}
	}
	for (int i = 0; i < nrules; i++) {
		fscanf(date, " %c", &simbol[i]);
		fscanf(date, " %s", succesor[i]);
	}
	printf("Loaded %s (L-system with %d rules)\n", fisier, nrules);
	fclose(date);
	(*cnt_undo)++;
	comanda_undo[*cnt_undo] = (comanda){0};
	comanda_undo[*cnt_undo].fisier = fisier;
	comanda_undo[*cnt_undo].nrules = nrules;
	comanda_undo[*cnt_undo].axioma = axioma;
	comanda_undo[*cnt_undo].simbol = simbol;
	comanda_undo[*cnt_undo].succesor = succesor;
	free_redo(comanda_redo, *cnt_redo);
	*cnt_redo = -1;
}

/* Calculate and output the n-th derivation of the currently loaded L-System */
void derive(comanda *comanda_undo, int *cnt_undo, char *buff)
{
	int ok = -1; 
	for (int i = *cnt_undo; i >= 0 && ok == -1; i--) {
		if (comanda_undo[i].nrules != 0) {
			ok = i;
		}
	}
	if (ok == -1) {
		printf("No L-system loaded\n");
		return;
	}
	int nrules = comanda_undo[ok].nrules;
	char *axioma = comanda_undo[ok].axioma;
	char *simbol = comanda_undo[ok].simbol;
	char **succesor = comanda_undo[ok].succesor;
	int nr_derive;
	if (sscanf(buff, "DERIVE %d", &nr_derive) != 1) {
		return;
	}
	if (nr_derive == 0) {
		printf("%s\n", axioma);
		return;
	}
	char *derive = alocare_dinamica();
	strcpy(derive, axioma);
	for (int i = 0 ; i < nr_derive; i++) {
		char *aux = alocare_dinamica();
		size_t cnt = 50, len_aux = 0, len_derive = strlen(derive);
		aux[0] = '\0';
		for (size_t j = 0; j < len_derive; j++) {
			int ok = 0;
			for (int k = 0; k < nrules && ok == 0; k++) {
				if (simbol[k] == derive[j]) {
					size_t len_succesor = strlen(succesor[k]);
					if (len_aux + len_succesor >= cnt - 1) {
						cnt = cnt * 2;
						aux = realocare_dinamica(aux, cnt);
					}
					memcpy(aux + len_aux, succesor[k], len_succesor);
					len_aux += len_succesor;
					ok = 1;
				}
			}
			if (ok == 0) {
				if (len_aux + 1 >= cnt - 1) {
					cnt = cnt * 2;
					aux = realocare_dinamica(aux, cnt);
				}
				aux[len_aux++] = derive[j];
			}
		}
		aux[len_aux] = '\0';
		free_sir(&derive);
		derive = aux;
	}
	printf("%s\n", derive);
	free_sir(&derive);
}

/* Load a binary PPM (P6) image into memory */
void load(comanda *comanda_undo, comanda  *comanda_redo,
		  int *cnt_undo, int *cnt_redo, char *buff)
{
	char *fisier = alocare_dinamica();
	strcpy(fisier, buff + 5);
	fisier[strlen(fisier) - 1] = '\0';
	FILE *date = fopen(fisier, "rb");
	if (!date) {
		printf("Failed to load %s\n", fisier);
		free_sir(&fisier);
		return;
	}
	char semnatura[4];
	int width = 0, height = 0, maxim_canal = 0;
	fscanf(date, "%s", semnatura);
	fscanf(date, "%d %d", &width, &height);
	fscanf(date, "%d", &maxim_canal);
	fgetc(date);
	printf("Loaded %s (PPM image %dx%d)\n", fisier, width, height);
	unsigned char **pixeli = malloc(height * sizeof(unsigned char *));
	if (!pixeli) {
		printf("Eroare de alocare.\n");
	}
	for (int i = 0; i < height; i++) {
		pixeli[i] = malloc(width * 3 * sizeof(unsigned char));
		if (!pixeli[i]) {
			printf("Eroare la alocarea liniei.\n");
		} else {
			fread(pixeli[i], 1, width * 3, date);
		}
	}
	fclose(date);
	(*cnt_undo)++;
	comanda_undo[*cnt_undo] = (comanda){0};
	comanda_undo[*cnt_undo].fisier = fisier;
	comanda_undo[*cnt_undo].w = width;
	comanda_undo[*cnt_undo].h = height;
	comanda_undo[*cnt_undo].pixeli = pixeli;
	free_redo(comanda_redo, *cnt_redo);
	*cnt_redo = -1;
}

/* Write the current image state to a binary PPM (P6) file */
void save(comanda *comanda_undo, int *cnt_undo, char *buff)
{
	int ok = -1;
	for (int i = *cnt_undo; i >= 0 && ok == -1; i--) {
		if (comanda_undo[i].w != 0) {
			ok = i;
		}
	}
	if (ok == -1) {
		printf("No image loaded\n");
		return;
	}
	char *fisier = alocare_dinamica();
	strcpy(fisier, buff + 5);
	fisier[strlen(fisier) - 1] = '\0';
	FILE *date = fopen(fisier, "wb");
	if (!date) {
		printf("No image loaded\n");
		free_sir(&fisier);
		return;
	}
	fprintf(date, "P6\n");
	fprintf(date, "%d %d\n", comanda_undo[ok].w, comanda_undo[ok].h);
	fprintf(date, "255\n");
	for (int i = 0; i < comanda_undo[ok].h; i++) {
		fwrite(comanda_undo[ok].pixeli[i], 1, comanda_undo[ok].w * 3, date);
	}
	printf("Saved %s\n", fisier);
	fclose(date); free_sir(&fisier);
}

/* Helper function to compute the n-th derivation specifically for Turtle graphics */
char *turtle_derive(comanda *comanda_undo, int ok, int n)
{
	int nrules = comanda_undo[ok].nrules;
	char *axioma = comanda_undo[ok].axioma;
	char *simbol = comanda_undo[ok].simbol;
	char **succesor = comanda_undo[ok].succesor;
	if (n == 0) {
		char *derive = malloc((strlen(axioma) + 1) * sizeof(char));
		strcpy(derive, axioma);
		return derive;
	}
	char *derive = alocare_dinamica();
	strcpy(derive, axioma);
	for (int i = 0 ; i < n; i++) {
		char *aux = alocare_dinamica();
		size_t cnt = 50, len_aux = 0, len_derive = strlen(derive);
		aux[0] = '\0';
		for (size_t j = 0; j < len_derive; j++) {
			int ok = 0;
			for (int k = 0; k < nrules && ok == 0; k++) {
				if (simbol[k] == derive[j]) {
					size_t len_succesor = strlen(succesor[k]);
					if (len_aux + len_succesor >= cnt - 1) {
						cnt = cnt * 2;
						aux = realocare_dinamica(aux, cnt);
					}
					memcpy(aux + len_aux, succesor[k], len_succesor);
					len_aux += len_succesor;
					ok = 1;
				}
			}
			if (ok == 0) {
				if (len_aux + 1 >= cnt - 1) {
					cnt = cnt * 2;
					aux = realocare_dinamica(aux, cnt);
				}
				aux[len_aux++] = derive[j];
			}
		}
		aux[len_aux] = '\0';
		free_sir(&derive);
		derive = aux;
	}
	return derive;
}

/* Rasterize and draw a line on the image matrix using Bresenham's line algorithm */
void desenare_linie(int x0, int y0, int x1, int y1, int R, int G, int B,
					int width, int height, unsigned char **pixeli)
{
	int dx = abs(x1 - x0);
	int sx = (x0 < x1) ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;
	while (1) {
		if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
			int rand = height - 1 - y0;
			pixeli[rand][3 * x0] = R;
			pixeli[rand][3 * x0 + 1] = G;
			pixeli[rand][3 * x0 + 2] = B;
		}
		if (x0 == x1 && y0 == y1) {
			break;
		}
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

/* Execute spatial movements and render fractal shapes based on L-System string */
void turtle(comanda *comanda_undo, comanda  *comanda_redo,
			int *cnt_undo, int *cnt_redo, char *buff)
{
	int ok1 = -1; 
	for (int i = *cnt_undo; i >= 0 && ok1 == -1; i--) {
		if (comanda_undo[i].axioma) {
			ok1 = i;
		}
	}
	if (ok1 == -1) {
		printf("No L-system loaded\n");
		return;
	}
	int ok2 = -1; 
	for (int i = *cnt_undo; i >= 0 && ok2 == -1; i--) {
		if (comanda_undo[i].pixeli) {
			ok2 = i;
		}
	}
	if (ok2 == -1) {
		printf("No image loaded\n");
		return;
	}
	double x, y, pas_deplasare, orientare, pas_unghiular;
	int n, R, G, B;
	if (sscanf(buff + 7, "%lf %lf %lf %lf %lf %d %d %d %d", &x, &y,
			   &pas_deplasare, &orientare, &pas_unghiular, &n, &R, &G,
			  &B) != 9) {
		return;
	}
	char *derive = turtle_derive(comanda_undo, ok1, n);
	unsigned char **pixeli = malloc(comanda_undo[ok2].h * sizeof(unsigned char *));
	if (!pixeli) {
		printf("Eroare de alocare.\n");
	}
	for (int i = 0; i < comanda_undo[ok2].h; i++) {
		pixeli[i] = malloc(comanda_undo[ok2].w * 3 * sizeof(unsigned char));
		if (!pixeli[i]) {
			printf("Eroare la alocarea liniei.\n");
		} else {
			memcpy(pixeli[i], comanda_undo[ok2].pixeli[i],
				   comanda_undo[ok2].w * 3);
		}
	}
	stare_testoasa *stare = malloc(MAX * sizeof(stare_testoasa));
	int cnt = MAX, stare_max = -1;
	for (size_t i = 0; i < strlen(derive); i++) {
		if (cnt - 1 <= stare_max) {
			stare = realocare_stare(stare, cnt);
			cnt = cnt * 2;
		}
		char c = derive[i];
		if (c == 'F') {
			double x1 = x + pas_deplasare * cos(orientare * PI / 180.0);
			double y1 = y + pas_deplasare * sin(orientare * PI / 180.0);
			desenare_linie((int)lround(x), (int)lround(y), (int)lround(x1),
						   (int)lround(y1), R, G, B, comanda_undo[ok2].w,
						   comanda_undo[ok2].h, pixeli);
			x = x1; y = y1;
		} else if (c == '+') {
			orientare += pas_unghiular;
		} else if (c == '-') {
			orientare -= pas_unghiular;
		} else if (c == '[') {
			stare[++stare_max] = (stare_testoasa){x, y, orientare};
		} else if (c == ']') {
			if (stare_max >= 0) {
				x = stare[stare_max].x; y = stare[stare_max].y;
				orientare = stare[stare_max--].orientare;
			}
		}
	}
	printf("Drawing done\n");
	comanda_undo[++(*cnt_undo)] = (comanda){0};
	comanda_undo[*cnt_undo].w = comanda_undo[ok2].w;
	comanda_undo[*cnt_undo].h = comanda_undo[ok2].h;
	comanda_undo[*cnt_undo].pixeli = pixeli;
	free_redo(comanda_redo, *cnt_redo); free_sir(&derive); free(stare);
	*cnt_redo = -1;
}

/* Parse glyph metadata and hex bitmaps from the BDF font format */
void caracter_date(FILE *date, char *citire, chars *caracter, int i)
{
	if (strncmp(citire, "ENCODING ", 9) == 0) {
		if (sscanf(citire + 9, "%d", &caracter[i].cod) != 1) {
			return;
		}
	}
	if (strncmp(citire, "DWIDTH ", 7) == 0) {
		if (sscanf(citire + 7, "%d %d", &caracter[i].dwx,
				   &caracter[i].dwy) != 2) {
			return;
		}
	}
	if (strncmp(citire, "BBX ", 4) == 0) {
		if (sscanf(citire + 4, "%d %d %d %d", &caracter[i].bbw,
				   &caracter[i].bbh, &caracter[i].bbxoff,
				   &caracter[i].bbyoff) != 4) {
			return;
		}
	}
	if (strncmp(citire, "BITMAP", 6) == 0) {
		caracter[i].bitmaps = malloc(caracter[i].bbh * sizeof(unsigned char *));
		int biti_rand = (caracter[i].bbw + 7) / 8;
		
		for (int j = 0; j < caracter[i].bbh; j++) {
			fgets(citire, MAX, date);
			caracter[i].bitmaps[j] = malloc(biti_rand);
			for (int b = 0; b < biti_rand; b++) {
				unsigned int bit;
				if (sscanf(citire + (b * 2), "%2x", &bit) != 1) {
					return;
				}
				caracter[i].bitmaps[j][b] = (unsigned char)bit;
			}
		}
	}
}

/* Open and parse a BDF font file into memory */
void font(comanda *comanda_undo, comanda  *comanda_redo,
		  int *cnt_undo, int *cnt_redo, char *buff)
{
	char *fisier = alocare_dinamica();
	strcpy(fisier, buff + 5);
	fisier[strlen(fisier) - 1] = '\0';
	FILE *date = fopen(fisier, "rt");
	if (!date) {
		printf("Failed to load %s\n", fisier);
		free_sir(&fisier);
		return;
	}
	char *citire = alocare_dinamica(), *nume = alocare_dinamica();
	int nglyphs;
	fgets(citire, MAX, date);
	do {
		if (strncmp(citire, "FONT ", 5) == 0) {
			strcpy(nume, citire + 5);
			int len = strlen(nume);
			if (len == MAX - 6 && nume[len - 1] != '\n') {
				int cnt = MAX;
				char c;
				while ((c = fgetc(date)) != '\n') {
					if (cnt - 1 == len) {
						nume = realocare_dinamica(nume, cnt);
						cnt = cnt * 2;
					}
					nume[len++] = c;
				}
				nume[len] = '\0';
			} else {
				nume[len - 1] = '\0';
			}
		}
		fgets(citire, MAX, date);
	} while (strncmp(citire, "CHARS ", 6));
	
	if (sscanf(citire + 6, "%d", &nglyphs) != 1) {
		return;
	}
	chars *caracter = malloc(nglyphs * sizeof(chars));
	for (int i = 0; i < nglyphs; i++) {
		fgets(citire, MAX, date); 
		while (strncmp(citire, "ENDCHAR", 7)) {
			caracter_date(date, citire, caracter, i);
			fgets(citire, MAX, date);
		}
	}
	printf("Loaded %s (bitmap font %s)\n", fisier, nume);
	fclose(date);
	(*cnt_undo)++;
	comanda_undo[*cnt_undo] = (comanda){0};
	comanda_undo[*cnt_undo].caracter = caracter;
	comanda_undo[*cnt_undo].fisier = fisier;
	comanda_undo[*cnt_undo].nume_font = nume;
	comanda_undo[*cnt_undo].nglyph = nglyphs;
	free_redo(comanda_redo, *cnt_redo); free_sir(&citire);
	*cnt_redo = -1;
}

/* Map the bits of a character's bitmap onto the 2D pixel image */
void desenare_caracter(int h, int w, chars *glyph, double x, double y,
					   int R, int G, int B, unsigned char **pixeli)
{
	for (int rand = 0; rand < (*glyph).bbh; rand++) {
		for (int coloana = 0; coloana < (*glyph).bbw; coloana++) {
			int byte = coloana / 8;
			int bit = 7 - (coloana % 8); 
			if ((*glyph).bitmaps[rand][byte] & (1 << bit)) {
				int x1 = x + (*glyph).bbxoff + coloana;
				int y1 = y + (*glyph).bbyoff + (*glyph).bbh - 1 - rand;
				if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h) {
					int linie = h - 1 - y1;
					pixeli[linie][3 * x1]     = (unsigned char)R;
					pixeli[linie][3 * x1 + 1] = (unsigned char)G;
					pixeli[linie][3 * x1 + 2] = (unsigned char)B;
				}
			}
		}
	}
}

/* Render a complete text string using the currently loaded bitmap font */
void type(comanda *comanda_undo, comanda  *comanda_redo,
		  int *cnt_undo, int *cnt_redo, char *buff)
{
	int ok1 = -1; 
	for (int i = *cnt_undo; i >= 0 && ok1 == -1; i--) {
		if (comanda_undo[i].pixeli) {
			ok1 = i;
		}
	}
	if (ok1 == -1) {
		printf("No image loaded\n");
		return;
	}
	int ok2 = -1; 
	for (int i = *cnt_undo; i >= 0 && ok2 == -1; i--) {
		if (comanda_undo[i].nume_font) {
			ok2 = i;
		}
	}
	if (ok2 == -1) {
		printf("No font loaded\n");
		return;
	}
	int h = comanda_undo[ok1].h, w = comanda_undo[ok1].w;
	char *mesaj = alocare_dinamica();
	int x, y, R, G, B;
	char *inceput_mesaj = strchr(buff, '"');
	char *sfarsit_mesaj = strrchr(buff, '"');
	int distanta = sfarsit_mesaj - inceput_mesaj - 1;
	
	if (distanta > 0) {
		strncpy(mesaj, inceput_mesaj + 1, distanta);
		mesaj[distanta] = '\0';
	} else {
		mesaj[0] = '\0';
	}
	
	if (sscanf(sfarsit_mesaj + 1, "%d %d %d %d %d", &x, &y, &R, &G, &B) != 5) {
		free(mesaj);
		return;
	}
	unsigned char **pixeli = malloc(h * sizeof(unsigned char *));
	if (!pixeli) {
		printf("Eroare de alocare.\n");
	}
	for (int i = 0; i < h; i++) {
		pixeli[i] = malloc(w * 3 * sizeof(unsigned char));
		if (!pixeli[i]) {
			printf("Eroare la alocarea liniei.\n");
		} else {
			memcpy(pixeli[i], comanda_undo[ok1].pixeli[i], w * 3);
		}
	}
	chars *font = comanda_undo[ok2].caracter;
	for (size_t c = 0; c < strlen(mesaj); c++) {
		int cod = (unsigned char)mesaj[c];
		chars *glyph = NULL;
		for (int i = 0; i < comanda_undo[ok2].nglyph; i++) {
			if (font[i].cod == cod) {
				glyph = &font[i];
				break;
			}
		}
		if (glyph) {
			desenare_caracter(h, w, glyph, x, y, R, G, B, pixeli);
			x += (*glyph).dwx;
			y += (*glyph).dwy;
		}
	}
	printf("Text written\n");
	(*cnt_undo)++;
	comanda_undo[*cnt_undo] = (comanda){0};
	comanda_undo[*cnt_undo].w = comanda_undo[ok1].w;
	comanda_undo[*cnt_undo].h = comanda_undo[ok1].h;
	comanda_undo[*cnt_undo].pixeli = pixeli;
	comanda_undo[*cnt_undo].nglyph = comanda_undo[ok2].nglyph;
	free_redo(comanda_redo, *cnt_redo); free_sir(&mesaj);
	*cnt_redo = -1;
}

/* Analyze the binary pixel stream for specific read-error vulnerabilities (0010, 1101) */
void bitcheck(comanda *comanda_undo, int *cnt_undo)
{
	int ok = -1; 
	for (int i = *cnt_undo; i >= 0 && ok == -1; i--) {
		if (comanda_undo[i].pixeli) {
			ok = i;
		}
	}
	if (ok == -1) {
		printf("No image loaded\n");
		return;
	}
	int w = comanda_undo[ok].w;
	int h = comanda_undo[ok].h;
	long bytes = w * h * 3;
	unsigned char *pixeli = malloc(bytes);
	for (int i = 0; i < h; i++) {
		memcpy(pixeli + (i * w * 3), comanda_undo[ok].pixeli[i], w * 3);
	}
	long biti = bytes * 8;
	for (long i = 0; i < biti - 3; i++) {
		int bit[4];
		for (int k = 0; k < 4; k++) {
			long bit_poz_pix = i + k; 
			long byte = bit_poz_pix / 8; 
			int bit_poz = 7 - (bit_poz_pix % 8); 
			bit[k] = (pixeli[byte] >> bit_poz) & 1;
		}
		
		if ((bit[0] == 0 && bit[1] == 0 && bit[2] == 1 && bit[3] == 0) ||
			(bit[0] == 1 && bit[1] == 1 && bit[2] == 0 && bit[3] == 1)) {
			long bit_poz = i + 2; 
			long byte = bit_poz / 8; 
			int bit = 7 - (bit_poz % 8); 
			long pixel = byte / 3; 
			int culoare = byte % 3; 
			int coloana = pixel % w;
			int linie = pixel / w;
			
			unsigned char R = comanda_undo[ok].pixeli[linie][3 * coloana];
			unsigned char G = comanda_undo[ok].pixeli[linie][3 * coloana + 1];
			unsigned char B = comanda_undo[ok].pixeli[linie][3 * coloana + 2];
			
			if (culoare == 0) R ^= (1 << bit);
			if (culoare == 1) G ^= (1 << bit);
			if (culoare == 2) B ^= (1 << bit);
			
			printf("Warning: pixel at (%d, %d) may be read as (%d, %d, %d)\n",
				   coloana, h - 1 - linie, R, G, B);
		}
	}
	if (pixeli) {
		free(pixeli);
		pixeli = NULL;
	}
}

/* Main command loop and entry point */
int main(void)
{
	char *buff = alocare_dinamica();
	int cnt_undo = -1, cnt_redo = -1, cnt_u = MAX, cnt_r = MAX;
	comanda *comanda_undo = malloc(MAX * sizeof(comanda));
	comanda *comanda_redo = malloc(MAX * sizeof(comanda));
	
	fgets(buff, MAX, stdin);
	do {
		if (cnt_u - 1 <= cnt_undo) {
			comanda_undo = realocare_comanda(comanda_undo, cnt_u);
			cnt_u = cnt_u * 2;
		}
		if (cnt_r - 1 <= cnt_redo) {
			comanda_redo = realocare_comanda(comanda_redo, cnt_r);
			cnt_r = cnt_r * 2;
		}
		
		if (strncmp(buff, "UNDO", 4) == 0) {
			undo(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo);
		}
		if (strncmp(buff, "REDO", 4) == 0) {
			redo(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo);
		}
		if (strncmp(buff, "LSYSTEM", 7) == 0) {
			lsystem(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo, buff);
		}
		if (strncmp(buff, "DERIVE", 6) == 0) {
			derive(comanda_undo, &cnt_undo,  buff);
		}
		if (strncmp(buff, "LOAD", 4) == 0) {
			load(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo, buff);
		}
		if (strncmp(buff, "SAVE", 4) == 0) {
			save(comanda_undo, &cnt_undo,  buff);
		}
		if (strncmp(buff, "TURTLE", 6) == 0) {
			turtle(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo, buff);
		}
		if (strncmp(buff, "FONT", 4) == 0) {
			font(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo, buff);
		}
		if (strncmp(buff, "TYPE", 4) == 0) {
			type(comanda_undo, comanda_redo, &cnt_undo, &cnt_redo, buff);
		}
		if (strncmp(buff, "BITCHECK", 8) == 0) {
			bitcheck(comanda_undo, &cnt_undo);
		}
		fgets(buff, MAX, stdin);
	} while (strncmp(buff, "EXIT", 4));
	
	free_undo(comanda_undo, cnt_undo); 
	free_redo(comanda_redo, cnt_redo);
	free_sir(&buff); 
	free(comanda_undo); 
	free(comanda_redo);
	
	return 0;
}
