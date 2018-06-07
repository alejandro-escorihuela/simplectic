/* 17-04-2018 */
/* alex */
/* simp.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0, gT, gV;
  real h;
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  
  /* Mètode simplèctic d'Euler TV */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Càlcul de les q(n + 1)*/
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	gT = (p[i][j] / masses[i]);
	q[i][j] += h * gT;
      }
    }
    /* Càlcul de les p(n + 1)*/
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	gV = gradV(masses, q, i, j, planetes);
  	p[i][j] -= h * gV;
      }
      Neval++;
    }
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
