/* 08-11-2018 */
/* alex */
/* metodes.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metodes.h"

void phi_T(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      q[i][j] += t * (p[i][j] / m[i]);
}

void phi_V(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      p[i][j] -= t * gradV(m, q, i, j, npl);
}

void phi_Vm(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t1, real t2) {
  real gV, gV2;
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++) {
      gradVmod(m, q, i, j, npl, &gV, &gV2);
      p[i][j] = p[i][j] - (t1 * gV) + (t2 * gV2);
    }
}

void phi_Tv(real m[MAX_PAR], real q[MAX_PAR][COMP], real v[MAX_PAR][COMP], int npl, real t) {
  int i, j;
  (void) m;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      q[i][j] += t * v[i][j];
}

void phi_Vv(real m[MAX_PAR], real q[MAX_PAR][COMP], real v[MAX_PAR][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] += t * deriv2q(m, q, i, j, npl);
}

void phi_simpTV(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  phi_T(m, q, p, npl, t);
  phi_V(m, q, p, npl, t);
}

void phi_simpVT(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  phi_V(m, q, p, npl, t);
  phi_T(m, q, p, npl, t);
}

void phi_stor(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  phi_V(m, q, p, npl, 0.5 * t);
  phi_T(m, q, p, npl, t);
  phi_V(m, q, p, npl, 0.5 * t);
}

void phi_storAdj(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  phi_T(m, q, p, npl, 0.5 * t);
  phi_V(m, q, p, npl, t);
  phi_T(m, q, p, npl, 0.5 * t);
}

void phi_storMod(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  phi_T(m, q, p, npl, 0.5 * t);
  phi_Vm(m, q, p, npl, t, (t * t * t) / 24.0);
  phi_T(m, q, p, npl, 0.5 * t);
}

void phi_H0(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  int i;
  for (i = 1; i < npl; i++)
    phi0(q[i], p[i], t, m[i]);
}

void phi_eV1(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++) 
    for (j = 0; j < COMP; j++)
      p[i][j] -= t * egradV(m, q, i, j, npl);
}

void copiar(real orig[MAX_PAR][COMP], real copi[MAX_PAR][COMP], int npl) {
  int i, j;
  for (i = 0; i < npl; i++)
    for (j = 0; j < COMP; j++)
      copi[i][j] = orig[i][j];
}

void p2v(real m[MAX_PAR], real p[MAX_PAR][COMP], real v[MAX_PAR][COMP], int npl) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] = p[i][j] / m[i];
}

void v2p(real m[MAX_PAR], real p[MAX_PAR][COMP], real v[MAX_PAR][COMP], int npl) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      p[i][j] = v[i][j] * m[i];
}

real dif_v(real v1[COMP], real v2[COMP]) {
  int j;
  real dif = 0.0, aux;
  for (j = 0; j < COMP; j++) {
    aux = v1[j] - v2[j];
    dif += (aux * aux);
  }
  return ARREL_Q(dif);
}

real dot(real v1[COMP], real v2[COMP]) {
  int i;
  real ret = 0.0;
  for (i = 0; i < COMP; i++)
    ret += (v1[i] * v2[i]);
  return ret;
}

real norm(real v[COMP]) {
  int i;
  real ret = 0.0;
  for (i = 0; i < COMP; i++)
    ret += (v[i] * v[i]);
  return ARREL_Q(ret);
}
