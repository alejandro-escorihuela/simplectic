/* 13-04-2018 */
/* alex */
/* solar.c */
#include <stdio.h>
#include <stdlib.h>
#include "solar.h"

int init_planetes(char * f_ini, real m[MAX_PAR], char noms[MAX_PAR][MAX_CAD], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP]) {
  int i = 1, j;
  char cadena[20], nom_fit[30];
  double lec[7];
#if TIPUS == 3
  char buf[128];
#endif
  FILE * fp;
  
  sprintf(nom_fit, "./cnf/%s.cnf", f_ini);
  fp = fopen(nom_fit, "r" );
  if (fp == NULL) {
    fputs("El fitxer amb les dades inicials no existeix\n", stderr);
    exit(1);
  }
  fscanf(fp, "%s %s %s %s %s %s %s %s", cadena, cadena, cadena, cadena, cadena, cadena, cadena, cadena);
  strcpy(noms[0], "sol");
  m[0] = SOL_MASSA;
  q[0][0] = q[0][1] = q[0][2] = p[0][0] = p[0][1] = p[0][2] = 0.0;
  while (!feof(fp)) {
    fscanf(fp, "%s %lf %lf %lf %lf %lf %lf %lf", noms[i], &lec[0], &lec[1], &lec[2], &lec[3], &lec[4], &lec[5], &lec[6]);
#if TIPUS == 3
    sprintf(buf, "%.18e", lec[0]);
    m[i] = strtoflt128(buf, NULL);
    for (j = 0; j < COMP; j++) {
      sprintf(buf, "%.18e", lec[j + 1]);
      q[i][j] = strtoflt128(buf, NULL);
      sprintf(buf, "%.18e", lec[j + 4]);
      p[i][j] = m[i] * strtoflt128(buf, NULL);
    }
#else
    m[i] = (real) lec[0];
    for (j = 0; j < COMP; j++) {
      q[i][j] = (real) lec[j + 1];
      p[i][j] = m[i] * ((real) lec[j + 4]);
    }
#endif
    i++;
  }
  fclose(fp);
  return i;
}

real gradVSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  int k, m;
  real gV = 0.0, resta[COMP], den;
  for (k = 0; k < np; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT * masses[i];
  return gV;
}

real egradVSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  int k, m;
  real gV = 0.0, resta[COMP], den;

  if (i == 0)
    return 0.0;
  
  for (k = 1; k < np; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];

      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT * masses[i];
  return gV;
}

real deriv2qSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np) {
  return -gradVSolar(masses, q, i, j, np) / masses[i];
}

void gradVmodSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  int k, l;
  real sum1[np][COMP], sum2[np][COMP], s1[COMP], s2[COMP], g2, r2[np], r3[np], r5[np], qq[np][COMP];
  
  for (k = 0; k < np; k++) {
    if (k != i) {
      for (l = 0; l < COMP; l++)
	qq[k][l] = (q[i][l] - q[k][l]);
      r2[k] = (qq[k][0] * qq[k][0]) + (qq[k][1] * qq[k][1]) + (qq[k][2] * qq[k][2]);
      r3[k] = POTENCIA(r2[k], 1.5);
      r5[k] = r3[k] * r2[k];
      //printf("%.15e %.15e %.15e\n", r2[k], r3[k], r5[k]);
    }
  }
  for (k = 0; k < np; k++) {
    if (k != i) {
      for (l = 0; l < COMP; l++) {
	sum1[k][l] = (-3.0 * qq[k][j] * qq[k][l] * masses[k]) / r5[k];
	if (l == j) {
	  sum1[k][l] += masses[k] / r3[k];
	}
	sum2[k][l] = (qq[k][l] * masses[k]) / r3[k];
      }
    }
    //printf("%.15e %.15e\n", sum1[k][0], sum2[k][0]);
  }

  s1[0] = s1[1] = s1[2] = s2[0] = s2[1] = s2[2] = 0.0;
  for (k = 0; k < np; k++)
    if (k != i) {
      for (l = 0; l < COMP; l++) {
	s1[l] += sum1[k][l];
	s2[l] += sum2[k][l];
      }
    }
  g2 = 0.0;
  for (l = 0; l < COMP; l++)
    g2 += s1[l] * s2[l];
  
  g2 *= 2.0 * GRAV_CNT2 * masses[i];
  *gV = gradVSolar(masses, q, i, j, np);
  *gV2 = g2;

  //printf("%.15e\n", g2);
  fputs("Potencial modificat no definit per al Sistema Solar\n", stderr);
  exit(1);
}

void phiKepler(real masses[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int i, real h, int np) {
  /* Sergio Blanes and Fernando Casas: A Concise Introduction to Geometric Numerical Integrator p[28,29]*/
  real q_ant[COMP], p_ant[COMP];
  real t, mu, r0, v02, u, a;
  real c, s, sig, psi, w, x, x_ant;
  real ff, gg, fp, gp, aux;
  real tol = 1e-12;
  int j;
  (void) np;

  if (i > 0) {
    for (j = 0; j < COMP; j++) {
      q_ant[j] = q[i][j];
      p_ant[j] = p[i][j];
    }

    t = h / masses[i];
    mu = GRAV_CNT * SOL_MASSA * masses[i] * masses[i];
    r0 = norm(q[i]);
    v02 = dot(p[i], p[i]);
    u = dot(q[i], p[i]);
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
    for (j = 0; j < COMP; j++) {
      q[i][j] = (ff * q_ant[j]) + (gg * p_ant[j]);
      p[i][j] = (fp * q_ant[j]) + (gp * p_ant[j]);
    }
  }
}

real energiaSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  int i, j, k;
  real cin = 0.0, pot = 0.0;
  real resta[COMP];
  for (i = 0; i < np; i++)
    cin += ((p[i][0] * p[i][0]) + (p[i][1] * p[i][1]) + (p[i][2] * p[i][2])) / m[i];
  cin *= 0.5;
  for (i = 0; i < np; i++)
    for (j = 0; j < i; j++) {
      for (k = 0; k < COMP; k++)
  	resta[k] = q[i][k] - q[j][k];
      pot += (m[i] * m[j]) / ARREL_Q((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]));
    }
  pot *= -GRAV_CNT;
  return (cin + pot);
}
