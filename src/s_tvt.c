/* 07-11-2018 */
/* alex */
/* s_tvt.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "metodes.h"
#include "solar.h"
#define LINIA_MAX 2000
#define NUM_MAX_COEF 100

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
  return 0;
}

