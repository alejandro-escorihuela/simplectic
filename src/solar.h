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
#ifndef TIPUS
#define TIPUS 1
#endif
#undef MAX_PLA
#define MAX_PLA 20
#undef MAX_CAD
#define MAX_CAD 40
#undef COMP
#define COMP 3
#undef COORD
#define COORD 2
#undef SOL_MASSA
#define SOL_MASSA 1.00000597682
#undef GRAV_CNT
#undef GRAV_CNT2
#define GRAV_CNT 0.000295912208286
#define GRAV_CNT2 8.756403501269705e-08
#define _DERIV2Q_(M, Q, I, J, P) ((-1.0 / M[I]) * gradV(M, Q, I, J, P))

#if TIPUS == 0
#define POTENCIA(A, B) powf(A, B)
#define ARREL_Q(A) sqrtf(A)
#elif TIPUS == 1
#define POTENCIA(A, B) pow(A, B)
#define ARREL_Q(A) sqrt(A)
#elif TIPUS == 2
#define POTENCIA(A, B) powl(A, B)
#define ARREL_Q(A) sqrtl(A)
#elif TIPUS == 3
#define POTENCIA(A, B) powq(A, B)
#define ARREL_Q(A) sqrtq(A)
#endif

#if TIPUS == 3
#define COSINUS(A) cosq(A)
#define SINUS(A) sinq(A)
#define ABSOLUT(A) fabsq(A)
#else
#define COSINUS(A) cos(A)
#define SINUS(A) sin(A)
#define ABSOLUT(A) fabs(A)
#endif

#if TIPUS == 0
typedef float real;
#elif TIPUS == 1
typedef double real;
#elif TIPUS == 2
typedef long double real;
#elif TIPUS == 3
typedef __float128 real;
#endif

void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini);
int carregar_planetes(char * f_ini, real masses[MAX_PLA], char noms[MAX_PLA][MAX_CAD], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP]);
real gradV(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
real egradV(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
real deriv2q(real m[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl);
real deriv(int k, real m[MAX_PLA], real x[COORD][MAX_PLA][COMP], int i, int j, int npl);
void llibre(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int npl, real * gV, real * gV2);
real energia(real masses[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl);
void copiar(real orig[MAX_PLA][COMP], real copi[MAX_PLA][COMP], int npl);
void p2v(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl);
void v2p(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl);
void phi_T(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_V(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_Tv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t);
void phi_Vv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t);
void phi_simpTV(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_simpVT(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_stor(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_storAdj(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_H0(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phi_eV1(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t);
void phiKepler(real q[COMP], real p[COMP], real h, real m);
real dif_v(real v1[COMP], real v2[COMP]);
real dot(real v1[COMP], real v2[COMP]);
real norm(real v[COMP]);
void obrir_fitxers(FILE * fitxers[MAX_PLA + 1], char noms[MAX_PLA][MAX_CAD], char * f_ini, char * metode, int npl);
void escriure_fitxers(FILE * fitxers[MAX_PLA + 1], int pop, real dia, real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], real H0, real H, int npl);
void tancar_fitxers(FILE * fitxers[MAX_PLA + 1], int npl);
void print_info(real h, double t, int eval, real error);
double temps();

#endif

