/* 20-06-2018 */
/* alex */
/* 16_nia5.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, k, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0, gV;
  real h;
  int s = 5;
  real a[s + 1], ah[s + 1];
  real b[s], bh[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  a[0] = a[5] = 0.0753469602698929L;
  a[1] = a[4] = 0.5179168546882568L;
  a[2] = a[3] = 0.5 - (a[0] + a[1]);
  b[0] = b[4] = 0.1902259393736766L;
  b[1] = b[3] = 0.8465240704435263L;
  b[2] = 1.0 - (2.0 * (b[0] + b[1]));
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
  }
  ah[5] = a[5] * h;
  
  /* Mètode d'escissió */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Bucle per a k */
    for (k = 0; k < s; k++) {
      for (i = 1; i < planetes; i++)
	phiKepler(q[i], p[i], ah[k], masses[i]);
      
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = egradV(masses, q, i, j, planetes);
	  p[i][j] -= bh[k] * gV;
	}
      }
    }
    for (i = 1; i < planetes; i++)
      phiKepler(q[i], p[i], ah[5], masses[i]);
    
    Neval += (s * (planetes - 1));
    t += temps() - t0;
    H = energia(masses, q, p, planetes);
    DH = fabs(H - H0);
    if (DH > Hemax)
      Hemax = DH;
    if ((it % pit) == 0)
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
