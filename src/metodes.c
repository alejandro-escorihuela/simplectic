/* 08-11-2018 */
/* alex */
/* metodes.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metodes.h"

void vec_coef_copia(double vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], unsigned int coef) {
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
