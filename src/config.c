/* 15-11-2018 */
/* alex */
/* config.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini, char * t_potencial, char * t_metode, char * f_coef) {
  FILE * fp = fopen("./cnf/param.cnf", "r");
  int i, param[3];
  char nom[20], arxiu[25];

  if (num != 7) {
    fprintf(stderr, "Executeu %s [potencial] [tipus de mètode] [fitxer de coeficients] [valor de h] [temps final] [iteracions]\n", vec[0]);
    exit(1);
  }
  if (fp == NULL) {
    fputs("El fitxer de parametres no existeix\n", stderr);
    exit(1);
  }
  fscanf(fp, "%s %s", nom, arxiu);
  i = 0;
  while (arxiu[i] != '.')
    i++;
  strncpy(f_ini, arxiu, i);
  f_ini[i] = '\0';
  for (i = 0; i < 2; i++)
    fscanf(fp, "%s %d", nom, &param[i]);
  
  *h = atof(vec[4]);
  *N = ceil(((real) atof(vec[5])) / *h);
  *pop = param[0];
  *pit = atoi(vec[6]);
  strcpy(t_potencial, vec[1]);
  strcpy(t_metode, vec[2]);
  sprintf(f_coef, "%s_%s", vec[2], vec[3]);
  fclose(fp);
}

double temps() {
  clock_t ct;
  struct tms t;
  ct = times(&t);
  if (ct == (clock_t) - 1)
    return 0.0;
  else
    return (((double) t.tms_utime) / ((double) sysconf(_SC_CLK_TCK)));
}

void vec_coef_copia(double vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef) {
#if TIPUS == 3
  char buf[128];
#endif
  int i;
  for (i = 0; i < coef; i++) {
#if TIPUS == 3
    sprintf(buf, "%lf", vec_orig[i]);
    vec_dest[i] = strtoflt128(buf, NULL);
#else
    vec_dest[i] = (real) vec_orig[i];
#endif
  }
}

void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], real vec_g[NUM_MAX_COEF], real * b_m, int * tam_a, int * tam_b, int * tam_y, int * tam_z, int * tam_g) {
  FILE * fp;
  char linia[LINIA_MAX], * valor, * tipus, nom_fit[30];
  double vec_aux[NUM_MAX_COEF];
  int i, tam_aux = 0;
  const char delim[2] = " ";
  
  *tam_a = *tam_b = *tam_y = *tam_z = 0;
  sprintf(nom_fit, "./coef/%s.cnf", f_coef);
  fp = fopen(nom_fit, "r" );
  if (fp == NULL) {
    fputs("El fitxer amb els coeficients no existeix\n", stderr);
    exit(1);
  }
  fgets(linia, LINIA_MAX, fp);
  while (!feof(fp)) {
    tipus = valor = strtok(linia, delim);
    valor = strtok(NULL, delim);
    i = 0;
    while (valor != NULL) {
      vec_aux[i] = atof(valor);
      valor = strtok(NULL, delim);
      i++;
    }
    tam_aux = i;
    if (strcmp(tipus, "a") == 0) {
      *tam_a = tam_aux;
      vec_coef_copia(vec_aux, vec_a, *tam_a);
    }
    else if (strcmp(tipus, "b") == 0) {
      *tam_b = tam_aux;
      vec_coef_copia(vec_aux, vec_b, *tam_b);
    }
    else if (strcmp(tipus, "y") == 0) {
      *tam_y = tam_aux;
      vec_coef_copia(vec_aux, vec_y, *tam_y);
    }
    else if (strcmp(tipus, "z") == 0) {
      *tam_z = tam_aux;
      vec_coef_copia(vec_aux, vec_z, *tam_z);
    }
    else if (strcmp(tipus, "g") == 0) {
      *tam_g = tam_aux;
      vec_coef_copia(vec_aux, vec_g, *tam_g);
    }
    else if (strcmp(tipus, "m") == 0)
      *b_m = vec_aux[0];
    fgets(linia, LINIA_MAX, fp);
  } 
  fclose(fp);

}

void obrir_fitxers(FILE * fitxers[MAX_PAR + 1], char noms[MAX_PAR][MAX_CAD], char * f_ini, char * potencial, char * metode, int np) {
  int i;
  char cad1[MAX_CAD], cad2[MAX_CAD];
  mkdir("./dat", 0755);
  if (strcmp(potencial, "solar") == 0)
    sprintf(cad1, "./dat/%s_%s_%s", potencial, f_ini, metode);
  else
    sprintf(cad1, "./dat/%s_%s", potencial, metode);
  mkdir(cad1, 0755);
  for (i = 0; i < np; i++) {
    sprintf(cad2, "%s/%c%c%c.dat", cad1, noms[i][0], noms[i][1], noms[i][2]);
    fitxers[i] = fopen(cad2, "w");
  }
  sprintf(cad2, "%s/err.dat", cad1);
  fitxers[np] = fopen(cad2, "w");
}

void tancar_fitxers(FILE * fitxers[MAX_PAR + 1], int np) {
  int i;
  for (i = 0; i <= np; i++)
    fclose(fitxers[i]);
}

void escriure_fitxers(FILE * fitxers[MAX_PAR + 1], int pop, real dia, real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], real H0, real H, int np) {
  int i, j;
#if TIPUS == 3
  char buf[128];
#endif
  if (pop < 2) {
#if TIPUS == 2
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	fprintf(fitxers[i], "%.24Le ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[np], "%.24Le %.24Le %.24Le\n", dia, H0, H);
#elif TIPUS == 3
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	quadmath_snprintf(buf, sizeof(buf), "%.34Qg", dia);
	fprintf(fitxers[i], "%s ", buf);
	for (j = 0; j < COMP; j++) {
	  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", q[i][j]);
	  fprintf(fitxers[i], "%s ", buf);
	}
	for (j = 0; j < COMP; j++) {
	  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", p[i][j]);
	  fprintf(fitxers[i], "%s ", buf);
	}
	fprintf(fitxers[i], "\n");
      }
    }
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", dia);
    fprintf(fitxers[np], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H0);
    fprintf(fitxers[np], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H);
    fprintf(fitxers[np], "%s\n", buf);
#else
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	fprintf(fitxers[i], "%.15e ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[np], "%.15e %.15e %.15e\n", dia, H0, H);
#endif
  }
}

void print_info(real h, double t, int eval, real error) {
#if TIPUS == 3
  char buf[128];
#endif
#if TIPUS == 2
  printf("%.5f %f %d %.24Le\n", (float) h, t, eval, error);
#elif TIPUS == 3
  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", error);
  printf("%.5f %f %d %s\n", (float) h, t, eval, buf);
#else
  printf("%.5f %f %d %.15e\n", (float) h, t, eval, error);
#endif
}
