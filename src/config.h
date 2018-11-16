/* 15-11-2018 */
/* alex */
/* metodes.h */
#ifndef _CONFIG_H
#define _CONFIG_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <quadmath.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "tipus.h"
#undef NUM_MAX_COEF
#undef LINIA_MAX
#define NUM_MAX_COEF 100
#define LINIA_MAX 2000

/* configuracio */
void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini, char * t_metode, char * f_coef);
double temps();


/* lectura de coeficients */
void vec_coef_copia(real vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], int * tam_a, int * tam_b, int * tam_y, int * tam_z);

#endif
