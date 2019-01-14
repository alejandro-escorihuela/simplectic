/* 14-01-2019 */
/* alex */
/* harmonic.h */
#ifndef _HARMONIC_H
#define _HARMONIC_H
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
#define MASSA_HAR 1.0
#define K_HAR 1.0

int init_harmonic(real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVharmonic(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVharmonic(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qharmonic(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phi0harmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np);
real energiaharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);

#endif

