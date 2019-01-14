/* 14-01-2019 */
/* alex */
/* harmonic.c */
#include <stdio.h>
#include <stdlib.h>
#include "harmonic.h"

int init_harmonic(real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int N = 1;
  double qx, px, massa = MASSA_HAR;
#if TIPUS == 3
  char buf[128];
#endif
  sprintf(noms[0], "pen");
  qx = 2.5;
  px = 0.0;
#if TIPUS == 3
  sprintf(buf, "%.18e", qx);
  q[0][0] = strtoflt128(buf, NULL);
  sprintf(buf, "%.18e", px);
  p[0][0] = strtoflt128(buf, NULL);
  sprintf(buf, "%.18e", massa);
  m[0] = strtoflt128(buf, NULL);
#else
  q[0][0] = (real) qx;
  p[0][0] = (real) px;
  m[0] = massa;
#endif
  return N;
}

real gradVharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  (void) i;
  (void) j;
  (void) np;
  return K_HAR * q[0][0];
}

real egradVharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  (void) q;
  (void) i;
  (void) j;
  (void) np;
  fputs("eH1 no definit per al potencial harmònic\n", stderr);
  exit(1);
  return masses[0];
}

real deriv2qharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVharmonic(masses, q, i, j, np) / masses[i];
}

void gradVmodharmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  (void) i;
  (void) j;
  (void) np;
  real g = K_HAR * q[0][0];
  *gV = g;
  *gV2 = (2.0 * K_HAR * g) / masses[0];
}

void phi0harmonic(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np) {
  (void) masses;
  (void) q;
  (void) p;
  (void) i;
  (void) h;
  (void) np;
  fputs("H0 no definit per al potencial harmònic\n", stderr);
  exit(1);
}

real energiaharmonic(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  (void) np;
  return ((0.5 * p[0][0] * p[0][0]) / m[0]) + (0.5 * K_HAR * q[0][0] * q[0][0]);
}
