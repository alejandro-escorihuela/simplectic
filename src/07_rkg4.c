/* 26-04-2018 */
/* alex */
/* 07_rkg4.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA];
  int k, cond;
  real x[COORD][MAX_PLA][COMP];
  real k1[COORD][MAX_PLA][COMP], k2[COORD][MAX_PLA][COMP];
  real Y1[COORD][MAX_PLA][COMP], Y2[COORD][MAX_PLA][COMP];
  real k1_ab[COORD][MAX_PLA][COMP], k2_ab[COORD][MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h, fac, a11, a12, a21, a22, b, h11, h12, h21, h22, hb, tol;
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, x[0], x[1]);
  H0 = energia(masses, x[0], x[1], planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  fac = ARREL_Q(3.0) / 6.0;
  a11 = a22 = 0.25;
  a12 = 0.25 + fac;
  a21 = 0.25 - fac;
  b = 0.5;
  h11 = h22 = h * a11;
  h12 = h * a12;
  h21 = h * a21;
  hb = h * b;
  tol = 1e-18;
  
  /* Mètode Runge-Kutta-Gauss-Legendre 4 */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    for (i = 1; i < planetes; i++) 
      for (j = 0; j < COMP; j++) 
    	for (k = 0; k < COORD; k++)
	  k1[k][i][j] = k2[k][i][j] = deriv(k, masses, x, i, j, planetes);
    do {
      for (i = 1; i < planetes; i++) 
	for (j = 0; j < COMP; j++) 
	  for (k = 0; k < COORD; k++) {
	    k1_ab[k][i][j] = k1[k][i][j];
	    k2_ab[k][i][j] = k2[k][i][j];
	  }
      for (i = 1; i < planetes; i++) 
	for (j = 0; j < COMP; j++) 
	  for (k = 0; k < COORD; k++) 
	    Y1[k][i][j] = x[k][i][j] + (h11 * k1[k][i][j]) + (h12 * k2[k][i][j]);
      for (i = 1; i < planetes; i++) 
	for (j = 0; j < COMP; j++) 
	  for (k = 0; k < COORD; k++) {
	    k1[k][i][j] = deriv(k, masses, Y1, i, j, planetes);
	    Y2[k][i][j] = x[k][i][j] + (h21 * k1[k][i][j]) + (h22 * k2[k][i][j]);
	  }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  for (k = 0; k < COORD; k++) {
	    k2[k][i][j] = deriv(k, masses, Y2, i, j, planetes);
	  }
	}
      }
      for (i = 1; i < planetes; i++)
	cond = (dif_v(k1[0][i], k1_ab[0][i]) < tol) && (dif_v(k2[0][i], k2_ab[0][i]) < tol) && (dif_v(k1[1][i], k1_ab[1][i]) < tol) && (dif_v(k2[1][i], k2_ab[1][i]) < tol);
      Neval += (2 * (planetes - 1));
    } while (!cond);
    
    for (i = 1; i < planetes; i++)
      for (j = 0; j < COMP; j++)
    	for (k = 0; k < COORD; k++)
    	  x[k][i][j] = x[k][i][j] + (hb * (k1[k][i][j] + k2[k][i][j]));
    
    t += temps() - t0;
    H = energia(masses, x[0], x[1], planetes);
    DH = fabs(H - H0);
    if (DH > Hemax)
      Hemax = DH;
    if ((it % pit) == 0)
      escriure_fitxers(fit_pl, pop, ((real) it) * h, x[0], x[1], H0, H, planetes);
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
