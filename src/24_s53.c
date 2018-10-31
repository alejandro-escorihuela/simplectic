/* 30-10-2018 */
/* alex */
/* 24_s53.c */
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
  int s = 3;
  real a[s + 1], ah[s + 1];
  real b[s], bh[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[3] = 0.6756035959798288L;
  a[1] = a[2] = 0.5L - a[0];
  b[0] = b[2] = 1.3512071919596576L;
  b[1] = 1.0L -(2.0 * b[0]);
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
      phi_T(masses, q, p, planetes, ah[i]);
      phi_V(masses, q, p, planetes, bh[i]);
    }
    phi_T(masses, q, p, planetes, ah[s]);

    Neval += ((s + 1) * (planetes - 1));
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
