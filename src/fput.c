/* 21-11-2018 */
/* alex */
/* fput.c */
#include <stdio.h>
#include <stdlib.h>
#include "fput.h"

int init_FPUT(real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int i, N = NUM_PART_FPUT;
  double q_aux[N], p_aux[N], massa = 1.0, dist = 0.25;
#if TIPUS == 3
  char buf[128];
#endif
  for (i = 0; i < N; i++)
    sprintf(noms[i], "%.3d", i);
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
  return (q[i][0] - q[i - 1][0]) - (q[i + 1][0] - q[i][0]) + (ALPHA_FPUT * (POTENCIA(q[i][0] - q[i - 1][0], ORDRE_FPUT - 1) - POTENCIA(q[i + 1][0] - q[i][0], ORDRE_FPUT - 1)));
}

real egradVFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  (void) masses;
  if ((j != 0) || (i == 0) || (i == (np - 1)))
    return 0.0;
  return (q[i][0] - q[i + 1][0] - q[i - 1][0]) + (ALPHA_FPUT * (POTENCIA(q[i][0] - q[i - 1][0], ORDRE_FPUT - 1) - POTENCIA(q[i + 1][0] - q[i][0], ORDRE_FPUT - 1)));
}

real deriv2qFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVFPUT(masses, q, i, j, np) / masses[i];
}

void gradVmodFPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  real xi0, xi1, xi2, t1, t2, t3, t4, a = (real) ORDRE_FPUT;
  if ((j != 0) || (i == 0) || (i == (np - 1)))
    *gV = *gV2 = 0.0;
  else {
    xi0 = q[i - 1][0];
    xi1 = q[i][0];
    xi2 = q[i + 1][0];
    t1 = POTENCIA(xi1 - xi0, ORDRE_FPUT - 2);
    t2 = POTENCIA(xi2 - xi1, ORDRE_FPUT - 2);
    t3 = POTENCIA(xi1 - xi0, ORDRE_FPUT - 1);
    t4 = POTENCIA(xi2 - xi1, ORDRE_FPUT - 1);
    *gV = (xi1 - xi0) - (xi2 - xi1) + t3 - t4;
    *gV2 = (2.0 * (2.0 + ((a - 1.0) * t1) + ((a - 1.0) * t2)) * ((2.0 * xi1) + t3 - xi0 - xi2 - t4)) / masses[i];
  }
}

void phi0FPUT(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np) {
  real q_ant, p_ant;
  (void) masses;

  if ((i > 0) && (i < (np - 1))) {
    q_ant = q[i][0];
    p_ant = p[i][0];
    q[i][0] = COSINUS(h) * q_ant + SINUS(h) * p_ant;
    p[i][0] = -SINUS(h) * q_ant + COSINUS(h) * p_ant;
  }
}

real energiaFPUT(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  int i, j;
  real cin = 0.0, sum1 = 0.0, sum2 = 0.0, aux1, aux2, terme, pot;
  
  for (i = 0; i < np; i++)
    cin += (p[i][0] * p[i][0]) / m[i];
  cin *= 0.5;
  for (i = 0; i < np; i++) {
    terme = q[i + 1][0] - q[i][0];
    aux1 = terme * terme;
    sum1 += aux1;
    aux2 = aux1 * terme;
    for (j = 3; j < ORDRE_FPUT; j++)
      aux2 *= terme;
    sum2 += aux2;
  }
  pot = (0.5 * sum1) + ((ALPHA_FPUT * sum2) / (real) ORDRE_FPUT);
  return (cin + pot);
}
