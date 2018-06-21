/* 13-04-2018 */
/* alex */
/* solar.c */
#include <stdio.h>
#include <stdlib.h>
#include "solar.h"

void carregar_configuracio(int num, char * vec[], real * h, int * N, int * pop, int * pit, char * f_ini) {
  FILE * fp = fopen("./cnf/param.cnf", "r");
  int i, param[3];
  char nom[20], arxiu[25];

  if (num != 2) {
    fprintf(stderr, "Executeu %s [valor de h]\n", vec[0]);
    exit(1);
  }
  if (fp == NULL) {
    fputs("El fitxer de parametres no existeix\n", stderr);
    exit(1);
  }
  fscanf(fp, "%s %s", nom, arxiu);
  i = 0;
  while (arxiu[i] != '.')
    i++;
  strncpy(f_ini, arxiu, i);
  f_ini[i] = '\0';
  for (i = 0; i < 3; i++)
    fscanf(fp, "%s %d", nom, &param[i]);
  *h = atof(vec[1]);
  *N = ceil(((real) param[0]) / *h);
  *pop = param[1];
  *pit = param[2];
  fclose(fp);
}

int carregar_planetes(char * f_ini, real m[MAX_PLA], char noms[MAX_PLA][MAX_CAD], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP]) {
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

real gradV(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int planetes) {
  int k, m;
  real gV = 0.0, resta[COMP], den;
  for (k = 0; k < planetes; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT * masses[i];
  return gV;
}

real egradV(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int planetes) {
  int k, m;
  real gV = 0.0, resta[COMP], den;
  for (k = 1; k < planetes; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT * masses[i];
  return gV;
}

real deriv2q(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int planetes) {
  int k, m;
  real gV = 0.0, resta[COMP], den;
  for (k = 0; k < planetes; k++)
    if (i != k) {
      for (m = 0; m < COMP; m++)
	resta[m] = q[i][m] - q[k][m];
      den = POTENCIA((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]), 1.5);
      gV += (masses[k] * (q[i][j] - q[k][j])) / den;
    }
  gV *= GRAV_CNT;
  return -gV;
}

real deriv(int k, real m[MAX_PLA], real x[COORD][MAX_PLA][COMP], int i, int j, int planetes) {
  return k == 0 ? x[1][i][j] / m[i] : -gradV(m, x[0], i, j, planetes);  
}

void llibre(real masses[MAX_PLA], real q[MAX_PLA][COMP], int i, int j, int planetes, real * gV, real * gV2) {
  int k, m;
  real resta[COMP], g1, g2, den, num;
  g1 = g2 = 0.0;
  for (k = 0; k < planetes; k++)
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

real energia(real m[MAX_PLA], real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], int planetes) {
  int i, j, k;
  real cin = 0.0, pot = 0.0;
  real resta[COMP];
  for (i = 0; i < planetes; i++)
    cin += ((p[i][0] * p[i][0]) + (p[i][1] * p[i][1]) + (p[i][2] * p[i][2])) / m[i];
  cin *= 0.5;
  for (i = 1; i < planetes; i++)
    for (j = 0; j < i; j++) {
      for (k = 0; k < COMP; k++)
  	resta[k] = q[i][k] - q[j][k];
      pot += (m[i] * m[j]) / ARREL_Q((resta[0] * resta[0]) + (resta[1] * resta[1]) + (resta[2] * resta[2]));
    }
  pot *= -GRAV_CNT;
  return (cin + pot);
}

void phiKepler(real q[COMP], real p[COMP], real h, real m) {
  /* Sergio Blanes and Fernando Casas: A Concise Introduction to Geometric Numerical Integrator p[28,29]*/
  real q_ant[COMP], p_ant[COMP];
  real t, mu, r0, v02, u, a;
  real c, s, sig, psi, w, x, x_ant;
  real ff, gg, fp, gp, aux;
  real tol = 1e-16;
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

void obrir_fitxers(FILE * fitxers[MAX_PLA + 1], char noms[MAX_PLA][MAX_CAD], char * f_ini, char * metode, int planetes) {
  int i;
  char cad1[MAX_CAD], cad2[MAX_CAD];
  mkdir("./dat", 0755);
  sprintf(cad1, "./dat/%s_%s", metode, f_ini);
  mkdir(cad1, 0755);
  for (i = 0; i < planetes; i++) {
    sprintf(cad2, "%s/%c%c%c.dat", cad1, noms[i][0], noms[i][1], noms[i][2]);
    fitxers[i] = fopen(cad2, "w");
  }
  sprintf(cad2, "%s/err.dat", cad1);
  fitxers[planetes] = fopen(cad2, "w");
}

void escriure_fitxers(FILE * fitxers[MAX_PLA + 1], int pop, real dia, real q[MAX_PLA][COMP], real p[MAX_PLA][COMP], real H0, real H, int planetes) {
  int i, j;
#if TIPUS == 3
  char buf[128];
#endif
  if (pop < 2) {
#if TIPUS == 2
    if (pop == 0) {
      for (i = 0; i < planetes; i++) {
	fprintf(fitxers[i], "%.24Le ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.24Le ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[planetes], "%.24Le %.24Le %.24Le\n", dia, H0, H);
#elif TIPUS == 3
    if (pop == 0) {
      for (i = 0; i < planetes; i++) {
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
    fprintf(fitxers[planetes], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H0);
    fprintf(fitxers[planetes], "%s ", buf);
    quadmath_snprintf(buf, sizeof(buf), "%.34Qg", H);
    fprintf(fitxers[planetes], "%s\n", buf);
#else
    if (pop == 0) {
      for (i = 0; i < planetes; i++) {
	fprintf(fitxers[i], "%.15e ", dia);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", q[i][j]);
	for (j = 0; j < COMP; j++)
	  fprintf(fitxers[i], "%.15e ", p[i][j]);
	fprintf(fitxers[i], "\n");
      }
    }
    fprintf(fitxers[planetes], "%.15e %.15e %.15e\n", dia, H0, H);
#endif
  }
}

void tancar_fitxers(FILE * fitxers[MAX_PLA + 1], int planetes) {
  int i;
  for (i = 0; i <= planetes; i++)
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

double temps() {
  clock_t ct;
  struct tms t;
  ct = times(&t);
  if (ct == (clock_t) - 1)
    return 0.0;
  else
    return (((double) t.tms_utime) / ((double) sysconf(_SC_CLK_TCK)));
}
