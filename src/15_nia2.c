/* 21-06-2018 */
/* alex */
/* 15_nia2.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s = 2;
  real a[s + 1], ah[s + 1];
  real b[s], bh[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[2] = (3.0 - ARREL_Q(3.0)) / 6.0;
  a[1] = 1.0 - (2.0 * a[0]);
  b[0] = b[1] = 0.5L;
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
  }
  ah[s] = a[s] * h;
  
  /* Bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();

    /* Composició del mètode */
    for (i = 0; i < s; i++) {
      phi_H0(masses, q, p, planetes, ah[i]);
      phi_eV1(masses, q, p, planetes, bh[i]);
    }
    phi_H0(masses, q, p, planetes, ah[s]);
    
    Neval += (s * (planetes - 1));
    t += temps() - t0;
    H = energia(masses, q, p, planetes);
    DH = ABSOLUT(H - H0);
    if (DH > Hemax)
      Hemax = DH;
    if ((it % pit) == 0)
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
