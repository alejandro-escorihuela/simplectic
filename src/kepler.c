/* 16-01-2019 */
/* alex */
/* kepler.c */
#include <stdio.h>
#include <stdlib.h>
#include "kepler.h"

int init_kepler(real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int i, N = 2;
  real e = 0.6;
  real q1 = 1.0 - e, q2 = 0.0, p1 = 0.0, p2 = ARREL_Q((1.0 + e) / (1.0 - e));
  
  for (i = 0; i < N; i++)
    sprintf(noms[i], "pl%d", i);
  q[0][0] = q1;
  q[1][0] = q2;
  p[0][0] = p1;
  p[1][0] = p2;
  m[0] = m[1] = 1.0;
  return N;
}

real gradVKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  real r2, r3, gV;
  (void) masses;
  (void) j;
  (void) np;
  r2 = (q[0][0] * q[0][0]) + (q[1][0] * q[1][0]);
  r3 = POTENCIA(r2, 1.5);
  gV = q[i][0] / r3;
  return gV;
}

real egradVKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  (void) q;
  (void) i;
  (void) j;
  (void) np;
  fputs("eH1 no definit per al potencial Kepler\n", stderr);
  exit(1);
  return masses[0];
}

real deriv2qKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVKepler(masses, q, i, j, np) / masses[i];
}

void gradVmodKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  real r2, r3, g1, g2;
  (void) masses;
  (void) j;
  (void) np;
  r2 = (q[0][0] * q[0][0]) + (q[1][0] * q[1][0]);
  r3 = POTENCIA(r2, 1.5);
  g1 = q[i][0] / r3;
  g2 = (-4.0 * g1) / r3;
  *gV = g1;
  *gV2 = g2;
}

void phi0Kepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np) {
  (void) masses;
  (void) q;
  (void) p;
  (void) i;
  (void) h;
  (void) np;
  fputs("H0 no definit per al potencial Kepler\n", stderr);
  exit(1);
}

real energiaKepler(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  (void) m;
  (void) np;
  return (0.5 * ((p[0][0] * p[0][0]) + (p[1][0] * p[1][0]))) - (1.0 / (ARREL_Q((q[0][0] * q[0][0]) + (q[1][0] * q[1][0]))));
}
