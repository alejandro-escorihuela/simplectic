/* 07-11-2018 */
/* alex */
/* s_tvt.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "solar.h"
#define LINIA_MAX 2000
#define NUM_MAX_COEF 100

void vector_copia(real vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], unsigned int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], unsigned int * tam_a, unsigned int * tam_b, unsigned int * tam_y, unsigned int * tam_z);

int main (int num_arg, char * vec_arg[]) {
  real vec_a[NUM_MAX_COEF], vec_b[NUM_MAX_COEF], vec_y[NUM_MAX_COEF], vec_z[NUM_MAX_COEF];
  unsigned int tam_a = 0, tam_b = 0, tam_y = 0, tam_z = 0;
  unsigned int i;

  lectura_coef("./coef/s_6_4.cnf", vec_a, vec_b, vec_y, vec_z, &tam_a, &tam_b, &tam_y, &tam_z);
  for (i = 0; i < tam_a; i++)
    printf("a[%d] = %.18f\n", i, vec_a[i]);
  for (i = 0; i < tam_b; i++)
    printf("b[%d] = %.18f\n", i, vec_b[i]);
  for (i = 0; i < tam_y; i++)
    printf("y[%d] = %.18f\n", i, vec_y[i]);
  for (i = 0; i < tam_z; i++)
    printf("z[%d] = %.18f\n", i, vec_z[i]);
}

void vector_copia(double vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], unsigned int coef) {
#if TIPUS == 3
  char buf[128];
#endif
  unsigned int i;
  for (i = 0; i < coef; i++) {
#if TIPUS == 3
    sprintf(buf, "%lf", vec_orig[i]);
    vec_dest[i] = strtoflt128(buf, NULL);
#else
    vec_dest[i] = (real) vec_orig[i];
#endif
  }
}

void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], unsigned int * tam_a, unsigned int * tam_b, unsigned int * tam_y, unsigned int * tam_z) {
  FILE * fp = fopen(f_coef, "r");
  char linia[LINIA_MAX], * valor, * tipus;
  double vec_aux[NUM_MAX_COEF];
  unsigned int i, tam_aux = 0;
  const char delim[2] = " ";
  
  *tam_a = *tam_b = *tam_y = *tam_z = 0;
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
      vector_copia(vec_aux, vec_a, *tam_a);
    }
    else if (strcmp(tipus, "b") == 0) {
      *tam_b = tam_aux;
      vector_copia(vec_aux, vec_b, *tam_b);
    }
    else if (strcmp(tipus, "y") == 0) {
      *tam_y = tam_aux;
      vector_copia(vec_aux, vec_y, *tam_y);
    }
    else if (strcmp(tipus, "z") == 0) {
      *tam_z = tam_aux;
      vector_copia(vec_aux, vec_z, *tam_z);
    }
    fgets(linia, LINIA_MAX, fp);
  } 
  fclose(fp);

}
