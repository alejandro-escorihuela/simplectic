/* 08-11-2018 */
/* alex */
/* tipus.h */
#ifndef _TIPUS_H
#define _TIPUS_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <quadmath.h>
#include "tipus.h"
#ifndef TIPUS
#define TIPUS 1
#endif
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

#undef MAX_CAD
#undef NUM_MAX_COEF
#undef LINIA_MAX
#undef COMP
#undef MAX_PAR
#define MAX_CAD 40
#define NUM_MAX_COEF 100
#define LINIA_MAX 2000
#define COMP 3
#define MAX_PAR 20
#endif
