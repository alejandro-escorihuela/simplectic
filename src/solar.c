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
    sprintf(buf, "%lf", lec[0]);
    m[i] = strtoflt128(buf, NULL);
    for (j = 0; j < COMP; j++) {
      sprintf(buf, "%lf", lec[j + 1]);
      q[i][j] = strtoflt128(buf, NULL);
      sprintf(buf, "%lf", lec[j + 4]);
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
  int k, m;
  real gV = 0.0, resta[COMP], den;
  for (k = 0; k < np; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT;
  return -gV;
}

void gradVmodSolar(real masses[MAX_PAR], real q[MAX_PAR][COMP], int i, int j, int np, real * gV, real * gV2) {
  int k, m;
  real resta[COMP], g1, g2, den, num;
  g1 = g2 = 0.0;
  for (k = 0; k < np; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      num = (masses[k] * (q[i][j] - q[k][j]));
      g1 += num / den;
      g2 += (masses[k] *  num) / (den * den);
    }
  g1 *= GRAV_CNT * masses[i];
  g2 *= -4.0 * GRAV_CNT2 * masses[i];
  //printf("%.15e %.15e\n", g1, g2); exit(1);
  *gV = g1;
  *gV2 = g2;
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

real energiaSolar(real m[MAX_PAR], real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], int np) {
  int i, j, k;
  real cin = 0.0, pot = 0.0;
  real resta[COMP];
  for (i = 0; i < np; i++)
    cin += ((p[i][0] * p[i][0]) + (p[i][1] * p[i][1]) + (p[i][2] * p[i][2])) / m[i];
  cin *= 0.5;
  for (i = 1; i < np; i++)
    for (j = 0; j < i; j++) {
      for (k = 0; k < COMP; k++)
  	resta[k] = q[i][k] - q[j][k];
      pot += (m[i] * m[j]) / ARREL_Q((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]));
    }
  pot *= -GRAV_CNT;
  return (cin + pot);
}

void obrir_fitxers(FILE * fitxers[MAX_PAR + 1], char noms[MAX_PAR][MAX_CAD], char * f_ini, char * metode, int np) {
  int i;
  char cad1[MAX_CAD], cad2[MAX_CAD];
  mkdir("./dat", 0755);
  sprintf(cad1, "./dat/%s_%s", metode, f_ini);
  mkdir(cad1, 0755);
  for (i = 0; i < np; i++) {
    sprintf(cad2, "%s/%c%c%c.dat", cad1, noms[i][0], noms[i][1], noms[i][2]);
    fitxers[i] = fopen(cad2, "w");
  }
  sprintf(cad2, "%s/err.dat", cad1);
  fitxers[np] = fopen(cad2, "w");
}

void escriure_fitxers(FILE * fitxers[MAX_PAR + 1], int pop, real dia, real q[MAX_PAR][COMP], real p[MAX_PAR][COMP], real H0, real H, int np) {
  int i, j;
#if TIPUS == 3
  char buf[128];
#endif
  if (pop < 2) {
#if TIPUS == 2
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	fprintf(fitxers[i], "%.24Le ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[np], "%.24Le %.24Le %.24Le\n", dia, H0, H);
#elif TIPUS == 3
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	quadmath_snprintf(buf, sizeof(buf), "%.34Qg", dia);
	fprintf(fitxers[i], "%s ", buf);
	for (j = 0; j < COMP; j++) {
	  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", q[i][j]);
	  fprintf(fitxers[i], "%s ", buf);
	}
	for (j = 0; j < COMP; j++) {
	  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", p[i][j]);
	  fprintf(fitxers[i], "%s ", buf);
	}
	fprintf(fitxers[i], "\n");
      }
    }
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", dia);
    fprintf(fitxers[np], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H0);
    fprintf(fitxers[np], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H);
    fprintf(fitxers[np], "%s\n", buf);
#else
    if (pop == 0) {
      for (i = 0; i < np; i++) {
	fprintf(fitxers[i], "%.15e ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[np], "%.15e %.15e %.15e\n", dia, H0, H);
#endif
  }
}

void tancar_fitxers(FILE * fitxers[MAX_PAR + 1], int np) {
  int i;
  for (i = 0; i <= np; i++)
    fclose(fitxers[i]);
}

void print_info(real h, double t, int eval, real error) {
#if TIPUS == 3
  char buf[128];
#endif
#if TIPUS == 2
  printf("%.5f %f %d %.24Le\n", (float) h, t, eval, error);
#elif TIPUS == 3
  quadmath_snprintf(buf, sizeof(buf), "%.34Qg", error);
  printf("%.5f %f %d %s\n", (float) h, t, eval, buf);
#else
  printf("%.5f %f %d %.15e\n", (float) h, t, eval, error);
#endif
}

