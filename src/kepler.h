/* 16-01-2019 */
/* alex */
/* kepler.h */
#ifndef _KEPLER_H
#define _KEPLER_H
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



int init_kepler(real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVKepler(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVKepler(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qKepler(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phi0Kepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np);
real energiaKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);

#endif

