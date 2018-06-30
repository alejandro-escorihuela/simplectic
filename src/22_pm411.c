/* 30-06-2018 */
/* alex */
/* 22_pm411.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"
#define FACT_TEMP 1.05

int main (int num_arg, char * vec_arg[]){
  int i, j, k, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP], v[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0, gT, gV, gV2, d2q;
  real h, fac;
  int m = 3;
  real y[m], yh[m];
  real z[m], zh[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  fac = (h * h * h) / 24.0;
  
  /* coeficients */
  y[0] = 0.1859353996846055L;
  y[1] = 0.0731969797858114L;
  y[2] = -0.1576624269298081L;
  z[0] = 0.8749306155955435;
  z[1] = -0.237106680151022L;
  z[2] = -0.5363539829039128;
  for (i = 0; i < m; i++) {
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }
  
  /* p -> v */
  for (i = 1; i < planetes; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] = p[i][j] / masses[i];
  
  /* preprocessat */
  t0 = temps();
  for (k = 0; k < m; k++) {
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	d2q = deriv2q(masses, q, i, j, planetes);
	v[i][j] += yh[k] * d2q;
      }
    }
    for (i = 1; i < planetes; i++)
      for (j = 0; j < COMP; j++)
	q[i][j] += zh[k] * v[i][j];
  }
  Neval += (m * (planetes - 1));
  t += temps() - t0;
  
  /* v -> p */
  for (i = 1; i < planetes; i++)
    for (j = 0; j < COMP; j++)
      p[i][j] = v[i][j] * masses[i];    
  
  /* Mètode del llibre pàgina 31 */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Càlcul de les q(n + 0.5)*/
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	gT = (p[i][j] / masses[i]);
	q[i][j] += 0.5 * h * gT;
      }
    }
    /* Càlcul de les p(n + 1)*/
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	llibre(masses, q, i, j, planetes, &gV, &gV2);
  	p[i][j] = p[i][j] - (h * gV) + (fac * gV2);
      }
      Neval++;
    }
    /* Càlcul de les q(n + 1)*/
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	gT = (p[i][j] / masses[i]);
	q[i][j] += 0.5 * h * gT;
      }
    }   
    t += temps() - t0;
    /* imprimir */
    if ((it % pit) == 0) {
      /* p -> v */
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++)
	  v[i][j] = p[i][j] / masses[i];
      /* postprocessat */
      t0 = temps();
      for (k = m - 1; k >= 0; k--) {
	for (i = 1; i < planetes; i++) 
	  for (j = 0; j < COMP; j++)
	    q[i][j] += -zh[k] * v[i][j];
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    d2q = deriv2q(masses, q, i, j, planetes);
	    v[i][j] += -yh[k] * d2q;
	  }
	}
      }
      Neval += (m * (planetes - 1));
      t += temps() - t0;
      /* escriptura */
      /* v -> p */
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++)
	  p[i][j] = v[i][j] * masses[i];      
      H = energia(masses, q, p, planetes);
      DH = ABSOLUT(H - H0);
      if (DH > Hemax)
	Hemax = DH;
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
      /* preprocessat */
      t0 = temps();
      for (k = 0; k < m; k++) {
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    d2q = deriv2q(masses, q, i, j, planetes);
	    v[i][j] += yh[k] * d2q;
	  }
	}
	for (i = 1; i < planetes; i++)
	  for (j = 0; j < COMP; j++)
	    q[i][j] += zh[k] * v[i][j];
      }
      Neval += (m * (planetes - 1));
      t += temps() - t0;         
    }
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, ceil(((real ) Neval) * FACT_TEMP), Hemax / H0);
  return 0;
}
