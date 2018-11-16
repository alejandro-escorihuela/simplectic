/* 15-11-2018 */
/* alex */
/* config.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini, char * t_metode, char * f_coef) {
  FILE * fp = fopen("./cnf/param.cnf", "r");
  int i, param[3];
  char nom[20], arxiu[25];

  if (num != 5) {
    fprintf(stderr, "Executeu %s [tipus de mètode] [fitxer de coeficients] [valor de h] [temps final]\n", vec[0]);
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
  
  *h = atof(vec[3]);
  *N = ceil(((real) atof(vec[4])) / *h);
  *pop = param[0];
  *pit = param[1];
  strcpy(t_metode, vec[1]);
  sprintf(f_coef, "%s_%s", vec[1], vec[2]);
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

void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], int * tam_a, int * tam_b, int * tam_y, int * tam_z) {
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
    fgets(linia, LINIA_MAX, fp);
  } 
  fclose(fp);

}
