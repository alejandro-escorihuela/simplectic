/* 08-11-2018 */
/* alex */
/* metodes.h */
#ifndef _METODES_H
#define _METODES_H
#include <stdio.h>
#include <stdlib.h>
#include "solar.h"
#include "tipus.h"
#define NUM_MAX_COEF 100
#define LINIA_MAX 2000

void vec_coef_copia(real vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef);
void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], int * tam_a, int * tam_b, int * tam_y, int * tam_z);
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
void phiKepler(real q[COMP], real p[COMP], real h, real m);
#endif
