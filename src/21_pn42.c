/* 30-06-2018 */
/* alex */
/* 21_pn42.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, k, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP], v[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0, d2q;
  real h;
  int s = 2;
  int m = 3;
  real a[s], ah[s];
  real b[s], bh[s];
  real y[m], yh[m];
  real z[m], zh[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  a[0] = -0.1L;
  a[1] = 1.0 - a[0];
  b[0] = 0.5 - ARREL_Q(133.0 / 132.0);
  b[1] = 1.0 - b[0];
  y[0] = -0.1937696215758170L;
  y[1] = -0.931151146256426L;
  y[2] = 0.1053624334726687L;
  z[0] = -0.5349755290809216L;
  z[1] = 0.3086327690445878L;
  z[2] = -0.142837501141108L;
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
  }
  for (i = 0; i < m; i++) {
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }  
  
  for (i = 1; i < planetes; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] = p[i][j] / masses[i];
    
  /* Mètode d'escissió rkn */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* preprocessat */
    if ((it % pit) == 0) {
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
    }
    /* Mètode */
    for (k = 0; k < s; k++) {
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  d2q = deriv2q(masses, q, i, j, planetes);
	  v[i][j] += bh[k] * d2q;
	}
      }
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++)
	  q[i][j] += ah[k] * v[i][j];
    }
    Neval += (s * (planetes - 1));
    
    /* postprocessat */
    if ((it % pit) == 0) {
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
    }
    
    t += temps() - t0;

    if ((it % pit) == 0) {
      for (i = 1; i < planetes; i++)
	for (j = 0; j < COMP; j++)
	  p[i][j] = v[i][j] * masses[i];
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
