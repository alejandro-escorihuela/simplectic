/* 08-11-2018 */
/* alex */
/* metodes.h */
#ifndef _METODES_H
#define _METODES_H
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
#undef COMP
#undef MAX_PLA
#define NUM_MAX_COEF 100
#define LINIA_MAX 2000
#define COMP 3
#define MAX_PLA 20

/* variables globals per a potencials genèrics */
real (*gradV)(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
real (*egradV)(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
real (*deriv2q)(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
void (*gradVmod)(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl, real * gV, real * gV2);
void (*phi0)(real q[COMP], real p[COMP], real h, real m);
real (*energia)(real masses[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl);

void vec_coef_copia(real vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], int * tam_a, int * tam_b, int * tam_y, int * tam_z);

void copiar(real orig[MAX_PLA][COMP], real copi[MAX_PLA][COMP], int npl);
void p2v(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl);
void v2p(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl);
real dif_v(real v1[COMP], real v2[COMP]);
real dot(real v1[COMP], real v2[COMP]);
real norm(real v[COMP]);
double temps();


void phi_T(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_V(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_Vm(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t1, real t2);
void phi_Tv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t);
void phi_Vv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t);
void phi_simpTV(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_simpVT(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_stor(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_storAdj(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_storMod(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_H0(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_eV1(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
#endif
