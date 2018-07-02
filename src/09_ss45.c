/* 12-06-2018 */
/* alex */
/* 09_ss45.c */
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
  int s = 5;
  real a[s], ah[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[1] = a[3] = a[4] = 1.0 / (4.0 - POTENCIA(4.0, 1.0 / 3.0));
  a[2] = 1.0 - (4.0 * a[1]);
  for (i = 0; i < s; i++)
    ah[i] = a[i] * h;

  /* Bucle principal */
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Composició del mètode */
    for (i = 0; i < s; i++)
      phi_storAdj(masses, q, p, planetes, ah[i]);

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
