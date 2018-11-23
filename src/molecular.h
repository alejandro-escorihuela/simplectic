/* 19-11-2018 */
/* alex */
/* molecular.h */
#ifndef _MOLECULAR_H
#define _MOLECULAR_H
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
#undef EPSILON
#undef SIGMA
#undef SIGMA_DOT
#undef SIGMA_SIS
#undef BOLTZ
#undef MASSA_P
#define EPSILON 119.8
#define SIGMA 0.341
#define SIGMA_DOT 2.472023231929042220921962513681e-6
#define SIGMA_SIS 0.001572266908616041
#define BOLTZ 1.380658e-23
#define MASSA_P 66.34e-27

int init_molecules(real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVMolecular(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVMolecular(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qMolecular(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phi0Molecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np);
real temperaturaMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);
real energiaMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);

#endif

