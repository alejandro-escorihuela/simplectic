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

/* configuracio */
void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini, char * t_potencial, char * t_metode, char * f_coef);
double temps();

/* lectura de coeficients */
void vec_coef_copia(double vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], real vec_g[NUM_MAX_COEF], real * b_m, int * tam_a, int * tam_b, int * tam_y, int * tam_z, int * tam_g);

/* funcions comunes */
void obrir_fitxers(FILE * fitxers[MAX_PAR + 1], char noms[MAX_PAR][MAX_CAD], char * f_ini, char * potencial, char * metode, int np);
void tancar_fitxers(FILE * fitxers[MAX_PAR + 1], int np);
void escriure_fitxers(FILE * fitxers[MAX_PAR + 1], int pop, real dia, real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], real H0, real H, int np);
void print_info(real h, double t, int eval, real error);

#endif
