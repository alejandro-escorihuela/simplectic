/* 28-06-2018 */
/* alex */
/* 19_ps44.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, k, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0, gT, gV;
  real h;
  int s = 4;
  real a[s], ah[s];
  real b[s + 1], bh[s + 1];
  real y[s], yh[s];
  real z[s], zh[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  
  a[0] = a[3] = (57.0 + ARREL_Q(18069.0)) / 300.0;
  a[1] = a[2] = 0.5 - a[0];
  b[0] = b[4] = 6.0 / 25.0;
  b[1] = b[3] = -0.1L;
  b[2] = 1.0 - (2.0 * (b[0] + b[1]));
  y[0] = -0.5903105192555323L;
  y[1] = 0.0013732794565115L;
  y[2] = 0.3958521503201655L;
  y[3] = -(y[0] + y[1] + y[2]);
  z[0] = -0.117183575320267L;
  z[1] = 0.8785444960116207L;
  z[2] = -0.8972532123604465L;
  z[3] = -(z[0] + z[1] + z[2]);

  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }
  bh[s] = b[s] * h;
  
  /* Mètode d'escissió */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = 0; k < s; k++) {
	for (i = 1; i < planetes; i++) 
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += zh[k] * gT;
	  }
	for (i = 1; i < planetes; i++) 
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= yh[k] * gV;
	  }
      }
      Neval += (s * (planetes - 1));
    }
    /* Bucle per a k */
    for (k = 0; k < s; k++) {
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= bh[k] * gV;
	}
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += ah[k] * gT;
	}
    }
    for (i = 1; i < planetes; i++)
      for (j = 0; j < COMP; j++) {
	gV = gradV(masses, q, i, j, planetes);
	p[i][j] -= bh[s] * gV;
      }
    Neval += ((s + 1) * (planetes - 1));
  
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = s - 1; k >= 0; k--) {
	for (i = 1; i < planetes; i++)
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= -yh[k] * gV;
	  }
	for (i = 1; i < planetes; i++)
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += -zh[k] * gT;
	  }
      }
      Neval += (s * (planetes - 1));
    }
    
    t += temps() - t0;
    if ((it % pit) == 0) {
      H = energia(masses, q, p, planetes);
      DH = ABSOLUT(H - H0);
      if (DH > Hemax)
	Hemax = DH;      
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
    }
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
