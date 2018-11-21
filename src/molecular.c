/* 19-11-2018 */
/* alex */
/* molecular.c */
#include <stdio.h>
#include <stdlib.h>
#include "molecular.h"

int init_molecules(real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int i, j, N = 7;
  double q_aux[N][2], p_aux[N][2];
#if TIPUS == 3
  char buf[128];
#endif
  for (i = 0; i < N; i++)
    sprintf(noms[i], "pt%d", i);
  for (i = 0; i < N; i++)
    q[i][2] = p[i][2] = 0.0;
  q_aux[0][0] = 0.00;
  q_aux[0][1] = 0.00;
  q_aux[1][0] = 0.02;
  q_aux[1][1] = 0.39;
  q_aux[2][0] = 0.34;
  q_aux[2][1] = 0.17;
  q_aux[3][0] = 0.36;
  q_aux[3][1] = -0.21;
  q_aux[4][0] = -0.02;
  q_aux[4][1] = -0.40;
  q_aux[5][0] = -0.35;
  q_aux[5][1] = -0.16;
  q_aux[6][0] = -0.31;
  q_aux[6][1] = 0.21;
  p_aux[0][0] = MASSA_P * (-30.0);
  p_aux[0][1] = MASSA_P * (-20.0);
  p_aux[1][0] = MASSA_P * (+50.0);
  p_aux[1][1] = MASSA_P * (-90.0);
  p_aux[2][0] = MASSA_P * (-70.0);
  p_aux[2][1] = MASSA_P * (-60.0);
  p_aux[3][0] = MASSA_P * (+90.0);
  p_aux[3][1] = MASSA_P * (+40.0);
  p_aux[4][0] = MASSA_P * (+80.0);
  p_aux[4][1] = MASSA_P * (+90.0);
  p_aux[5][0] = MASSA_P * (-40.0);
  p_aux[5][1] = MASSA_P * (100.0);
  p_aux[6][0] = MASSA_P * (-80.0);
  p_aux[6][1] = MASSA_P * (-60.0);
#if TIPUS == 3
  for (i = 0; i < N; i++) {
    for (j = 0; j < 2; j++) {
      sprintf(buf, "%.18e", q_aux[i][j]);
      q[i][j] = strtoflt128(buf, NULL);
      sprintf(buf, "%.18e", p_aux[i][j]);
      p[i][j] = strtoflt128(buf, NULL);
    }
    sprintf(buf, "%.18e", MASSA_P);
    m[i] = strtoflt128(buf, NULL);
  }
#else
  for (i = 0; i < N; i++) {
    for (j = 0; j < 2; j++) {
      q[i][j] = (real) q_aux[i][j];
      p[i][j] = (real) p_aux[i][j];
    }
    m[i] = MASSA_P;
  }
#endif
  return N;
}

real gradVMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  int k, m;
  real gV = 0.0, resta[COMP], fac, r_dos, s2r2, s6r6, s12r12;
  (void) masses;
  for (k = 0; k < np; k++)
    if (i != k) {
      for (m = 0; m < 2; m++)
	resta[m] = q[i][m] - q[k][m];
      r_dos = (resta[0] * resta[0]) + (resta[1] * resta[1]);      
      s2r2 = SIGMA * SIGMA / r_dos;
      s6r6 = s2r2 * s2r2 * s2r2;
      s12r12 = s6r6 * s6r6;
      fac = (s6r6 - (2.0 * s12r12)) / r_dos;
      gV += fac * (q[i][j] - q[k][j]);
    }
  gV *= 24.0 * EPSILON * BOLTZ;
  return gV;
}

real egradVMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  (void) q;
  (void) i;
  (void) j;
  (void) np;
  fputs("eH1 no definit per al potencial molecular\n", stderr);
  exit(1);
  return masses[0];
}

real deriv2qMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVMolecular(masses, q, i, j, np) / masses[i];
}

void gradVmodMolecular(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  (void) masses;
  (void) q;
  (void) i;
  (void) j;
  (void) np;
  (void) gV;
  (void) gV2;
  fputs("Potencial molecular modificat no definit\n", stderr);
  exit(1);
}

void phi0Molecular(real q[COMP], real p[COMP], real h, real m) {
  (void) q;
  (void) p;
  (void) h;
  (void) m;
  fputs("H0 no definit per al potencial molecular\n", stderr);
  exit(1);
}

real temperaturaMolecular(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  /* no és un invariant exacte, fluctua al voltant d'un valor (22.72K) */
  int i;
  real sum = 0.0;
  real v[MAX_PAR][COMP];

  (void) q;
  p2v(m, p, v, np);
  for (i = 0; i < np; i++)
    sum += m[i] * ((v[i][0] * v[i][0]) + (v[i][1] * v[i][1]));
  return sum / (2.0 * ((real) np) * BOLTZ);
}

real energiaMolecular(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  int i, j, k;
  real cin = 0.0, pot = 0.0, resta[2], r_dos, s2r2, s6r6, s12r12;
  for (i = 0; i < np; i++)
    cin += ((p[i][0] * p[i][0]) + (p[i][1] * p[i][1])) / m[i];
  cin *= 0.5;
  for (i = 0; i < np; i++)
    for (j = 0; j < i; j++) {
      for (k = 0; k < COMP; k++)
  	resta[k] = q[i][k] - q[j][k];
      r_dos = (resta[0] * resta[0]) + (resta[1] * resta[1]);
      s2r2 = SIGMA * SIGMA / r_dos;
      s6r6 = s2r2 * s2r2 * s2r2;
      s12r12 = s6r6 * s6r6;
      pot += s12r12 - s6r6;
    }
  pot *= 4.0 * EPSILON * BOLTZ;
  return (cin + pot);
}
