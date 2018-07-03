/* 18-06-2018 */
/* alex */
/* 14_nb46.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP], v[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s = 6;
  real a[s], ah[s];
  real b[s + 1], bh[s + 1];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[5] = 0.2452989571842710L;
  a[1] = a[4] = 0.6048726657110800L;
  a[2] = a[3] = 0.5 - (a[0] + a[1]);
  b[0] = b[6] = 0.0829844064174052L;
  b[1] = b[5] = 0.3963098014983681L;
  b[2] = b[4] = -0.039056304922348L;
  b[3] = 1.0 - (2.0 * (b[0] + b[1] + b[2]));
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
  }
  bh[s] = b[s] * h;
  
  p2v(masses, p, v, planetes);
  
  /* Bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();

    /* Composició del mètode */
    for (i = 0; i < s; i++) {
      phi_Vv(masses, q, v, planetes, bh[i]);
      phi_Tv(masses, q, v, planetes, ah[i]);
    }
    phi_Vv(masses, q, v, planetes, bh[s]);
    v2p(masses, p, v, planetes);
    
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
