/* 08-11-2018 */
/* alex */
/* metodes.h */
#ifndef _METODES_H
#define _METODES_H
#include <stdio.h>
#include <stdlib.h>
#include "tipus.h"
#define NUM_MAX_COEF 100
#define LINIA_MAX 2000

void vec_coef_copia(real vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], unsigned int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], unsigned int * tam_a, unsigned int * tam_b, unsigned int * tam_y, unsigned int * tam_z);
#endif
