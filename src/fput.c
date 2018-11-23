/* 21-11-2018 */
/* alex */
/* fput.c */
#include <stdio.h>
#include <stdlib.h>
#include "fput.h"
int init_FPUT(real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int i, N = 10;
  double q_aux[N], p_aux[N], massa = 1.0, dist = 0.25;
#if TIPUS == 3
  char buf[128];
#endif
  for (i = 0; i < N; i++)
    sprintf(noms[i], "p%.2d", i);
  q_aux[0] = 0.0;
  for (i = 1; i < N; i++)
    q_aux[i] = q_aux[i - 1] + dist;
  for (i = 0; i < N; i++)
    p_aux[i] = 0.0;
  p_aux[5] = 0.1;
#if TIPUS == 3
  for (i = 0; i < N; i++) {
      sprintf(buf, "%.18e", q_aux[i]);
      q[i][0] = strtoflt128(buf, NULL);
      sprintf(buf, "%.18e", p_aux[i]);
      p[i][0] = strtoflt128(buf, NULL);
    sprintf(buf, "%.18e", massa);
    m[i] = strtoflt128(buf, NULL);
  }
#else
  for (i = 0; i < N; i++) {
    q[i][0] = (real) q_aux[i];
    p[i][0] = (real) p_aux[i];
    m[i] = massa;
  }
#endif
  return N;
}

real gradVFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  
  if ((j != 0) || (i == 0) || (i == (np - 1)))
    return 0.0;
  return (q[i][0] - q[i - 1][0]) - (q[i + 1][0] - q[i][0]) + POTENCIA(q[i][0] - q[i - 1][0], 2) - POTENCIA(q[i + 1][0] - q[i][0], 2);
}

real egradVFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  if ((j != 0) || (i == 0) || (i == (np - 1)))
    return 0.0;
  return (q[i][0] - q[i + 1][0] - q[i - 1][0]) + POTENCIA(q[i][0] - q[i - 1][0], 2) - POTENCIA(q[i + 1][0] - q[i][0], 2);
}

real deriv2qFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVFPUT(masses, q, i, j, np) / masses[i];
}

void gradVmodFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  (void) masses;
  (void) q;
  (void) i;
  (void) j;
  (void) np;
  (void) gV;
  (void) gV2;
  fputs("Potencial FPUT modificat no definit\n", stderr);
  exit(1);
}

void phi0FPUT(real q[COMP], real p[COMP], real h, real m) {
  real q_ant, p_ant;
  (void) m;
  
  q_ant = q[0];
  p_ant = p[0];
  q[0] = COSINUS(h) * q_ant + SINUS(h) * p_ant;
  p[0] = -SINUS(h) * q_ant + COSINUS(h) * p_ant;
}

real energiaFPUT(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  int i;
  real cin = 0.0, sum1 = 0.0, sum2 = 0.0, aux1, aux2, terme, pot;
  for (i = 0; i < np; i++)
    cin += (p[i][0] * p[i][0]) / m[i];
  cin *= 0.5;
  for (i = 0; i < np; i++) {
    terme = q[i + 1][0] - q[i][0];
    aux1 = terme * terme;
    sum1 += aux1;
    aux2 = aux1 * terme;
    sum2 += aux2;
  }
  pot = (0.5 * sum1) + (sum2 / 3.0);
  return (cin + pot);
}
