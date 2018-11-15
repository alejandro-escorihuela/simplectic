/* 13-04-2018 */
/* alex */
/* solar.h */
#ifndef _SOLAR_H
#define _SOLAR_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <quadmath.h>
#include <sys/stat.h>
#include <sys/times.h>
#include "tipus.h"
#include "metodes.h"
#undef MAX_CAD
#define MAX_CAD 40
#undef SOL_MASSA
#define SOL_MASSA 1.00000597682
#undef GRAV_CNT
#undef GRAV_CNT2
#define GRAV_CNT 0.000295912208286
#define GRAV_CNT2 8.756403501269705e-08
#define _DERIV2QSOLAR_(M, Q, I, J, P) ((-1.0 / M[I]) * gradVSolar(M, Q, I, J, P))

void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini, char * t_metode, char * f_coef);
int carregar_planetes(char * f_ini, real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phiKepler(real q[COMP], real p[COMP], real h, real m);
real energiaSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);
void obrir_fitxers(FILE * fitxers[MAX_PAR + 1], char noms[MAX_PAR][MAX_CAD], char * f_ini, char * metode, int np);
void escriure_fitxers(FILE * fitxers[MAX_PAR + 1], int pop, real dia, real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], real H0, real H, int np);
void tancar_fitxers(FILE * fitxers[MAX_PAR + 1], int np);
void print_info(real h, double t, int eval, real error);

#endif

