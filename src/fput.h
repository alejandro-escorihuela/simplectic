/* 21-11-2018 */
/* alex */
/* fput.h */
#ifndef _FPUT_H
#define _FPUT_H
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

int init_FPUT(real masses[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]);
real gradVFPUT(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real egradVFPUT(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
real deriv2qFPUT(real m[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np);
void gradVmodFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2);
void phi0FPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np);
real energiaFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np);

#endif

