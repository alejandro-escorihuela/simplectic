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
#undef SOL_MASSA
#define SOL_MASSA 1.00000597682
#undef GRAV_CNT
#undef GRAV_CNT2
#define GRAV_CNT 0.000295912208286
#define GRAV_CNT2 8.756403501269705e-08
#define _DERIV2QSOLAR_(M, Q, I, J, P) ((-1.0 / M[I]) * gradVSolar(M, Q, I, J, P))

int init_planetes(char * f_ini, real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phiKepler(real q[COMP], real p[COMP], real h, real m);
real energiaSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);

#endif

