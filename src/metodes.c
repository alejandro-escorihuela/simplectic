/* 08-11-2018 */
/* alex */
/* metodes.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metodes.h"

void vec_coef_copia(double vec_orig[NUM_MAX_COEF], real vec_dest[NUM_MAX_COEF], int coef) {
#if TIPUS == 3
  char buf[128];
#endif
  int i;
  for (i = 0; i < coef; i++) {
#if TIPUS == 3
    sprintf(buf, "%lf", vec_orig[i]);
    vec_dest[i] = strtoflt128(buf, NULL);
#else
    vec_dest[i] = (real) vec_orig[i];
#endif
  }
}

void lectura_coef(char * f_coef, real vec_a[NUM_MAX_COEF], real vec_b[NUM_MAX_COEF], real vec_y[NUM_MAX_COEF], real vec_z[NUM_MAX_COEF], int * tam_a, int * tam_b, int * tam_y, int * tam_z) {
  FILE * fp;
  char linia[LINIA_MAX], * valor, * tipus, nom_fit[30];
  double vec_aux[NUM_MAX_COEF];
  int i, tam_aux = 0;
  const char delim[2] = " ";
  
  *tam_a = *tam_b = *tam_y = *tam_z = 0;
  sprintf(nom_fit, "./coef/%s.cnf", f_coef);
  fp = fopen(nom_fit, "r" );
  if (fp == NULL) {
    fputs("El fitxer amb els coeficients no existeix\n", stderr);
    exit(1);
  }
  fgets(linia, LINIA_MAX, fp);
  while (!feof(fp)) {
    tipus = valor = strtok(linia, delim);
    valor = strtok(NULL, delim);
    i = 0;
    while (valor != NULL) {
      vec_aux[i] = atof(valor);
      valor = strtok(NULL, delim);
      i++;
    }
    tam_aux = i;
    if (strcmp(tipus, "a") == 0) {
      *tam_a = tam_aux;
      vec_coef_copia(vec_aux, vec_a, *tam_a);
    }
    else if (strcmp(tipus, "b") == 0) {
      *tam_b = tam_aux;
      vec_coef_copia(vec_aux, vec_b, *tam_b);
    }
    else if (strcmp(tipus, "y") == 0) {
      *tam_y = tam_aux;
      vec_coef_copia(vec_aux, vec_y, *tam_y);
    }
    else if (strcmp(tipus, "z") == 0) {
      *tam_z = tam_aux;
      vec_coef_copia(vec_aux, vec_z, *tam_z);
    }
    fgets(linia, LINIA_MAX, fp);
  } 
  fclose(fp);

}

void copiar(real orig[MAX_PLA][COMP], real copi[MAX_PLA][COMP], int npl) {
  int i, j;
  for (i = 0; i < npl; i++)
    for (j = 0; j < COMP; j++)
      copi[i][j] = orig[i][j];
}

void p2v(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] = p[i][j] / m[i];
}

void v2p(real m[MAX_PLA], real p[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl) {
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

double temps() {
  clock_t ct;
  struct tms t;
  ct = times(&t);
  if (ct == (clock_t) - 1)
    return 0.0;
  else
    return (((double) t.tms_utime) / ((double) sysconf(_SC_CLK_TCK)));
}

void phi_T(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      q[i][j] += t * (p[i][j] / m[i]);
}

void phi_V(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      p[i][j] -= t * gradV(m, q, i, j, npl);
}

void phi_Vm(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t1, real t2) {
  real gV, gV2;
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++) {
      gradVmod(m, q, i, j, npl, &gV, &gV2);
      p[i][j] = p[i][j] - (t1 * gV) + (t2 * gV2);
    }
}

void phi_Tv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t) {
  int i, j;
  (void) m;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      q[i][j] += t * v[i][j];
}

void phi_Vv(real m[MAX_PLA], real q[MAX_PLA][COMP], real v[MAX_PLA][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] += t * deriv2q(m, q, i, j, npl);
}

void phi_simpTV(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  phi_T(m, q, p, npl, t);
  phi_V(m, q, p, npl, t);
}

void phi_simpVT(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  phi_V(m, q, p, npl, t);
  phi_T(m, q, p, npl, t);
}

void phi_stor(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  phi_V(m, q, p, npl, 0.5 * t);
  phi_T(m, q, p, npl, t);
  phi_V(m, q, p, npl, 0.5 * t);
}

void phi_storAdj(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  phi_T(m, q, p, npl, 0.5 * t);
  phi_V(m, q, p, npl, t);
  phi_T(m, q, p, npl, 0.5 * t);
}

void phi_storMod(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  phi_T(m, q, p, npl, 0.5 * t);
  phi_Vm(m, q, p, npl, t, (t * t * t) / 24.0);
  phi_T(m, q, p, npl, 0.5 * t);
}

void phi_H0(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  int i;
  for (i = 1; i < npl; i++)
    phiKepler(q[i], p[i], t, m[i]);
}

void phi_eV1(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int npl, real t) {
  int i, j;
  for (i = 1; i < npl; i++) 
    for (j = 0; j < COMP; j++)
      p[i][j] -= t * egradV(m, q, i, j, npl);
}

void phiKepler(real q[COMP], real p[COMP], real h, real m) {
  /* Sergio Blanes and Fernando Casas: A Concise Introduction to Geometric Numerical Integrator p[28,29]*/
  real q_ant[COMP], p_ant[COMP];
  real t, mu, r0, v02, u, a;
  real c, s, sig, psi, w, x, x_ant;
  real ff, gg, fp, gp, aux;
  real tol = 1e-12;
  int i;

  for (i = 0; i < COMP; i++) {
    q_ant[i] = q[i];
    p_ant[i] = p[i];
  }

  t = h / m;
  mu = GRAV_CNT * SOL_MASSA * m * m;
  r0 = norm(q);
  v02 = dot(p, p);
  u = dot(q, p);
  a = -mu / (v02 - ((2.0 * mu) / r0));
  w = ARREL_Q(mu / (a * a * a));
  sig = 1 - r0 / a;
  psi = u / (w * a * a);
  
  x = x_ant = w * t * (a / r0);
  do {
    x_ant = x;
    c = COSINUS(x);
    s = SINUS(x);
    x = x - ((x - (sig * s) + (psi * (1.0 - c)) - (w * t)) / (1.0 - (sig * c) + (psi * s)));
  } while (ABSOLUT(x - x_ant) > tol);
  aux = 1.0 - (sig * c) + (psi * s);
  ff = 1.0 + (((c - 1.0) * a) / r0);
  gg = t + ((s - x) / w);
  fp = (-a * w * s) / (aux * r0);
  gp = 1.0 + ((c - 1) / aux);
  for (i = 0; i < COMP; i++) {
    q[i] = (ff * q_ant[i]) + (gg * p_ant[i]);
    p[i] = (fp * q_ant[i]) + (gp * p_ant[i]);
  }
}
