/* 12-06-2018 */
/* alex */
/* 11_ss69.c */
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
  int s = 17;
  real a[s], ah[s], ah2[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  a[0] = a[16] = 25.0 / 194.0;
  a[1] = a[15] = 0.581514087105251L;
  a[2] = a[14] = -0.410175371469850L;
  a[3] = a[13] = 0.1851469357165877L;
  a[4] = a[12] = -0.4095523434208514L;
  a[5] = a[11] = 0.1444059410800120L;
  a[6] = a[10] = 0.2783355003936797L;
  a[7] = a[9] = 0.3149566839162949L;
  a[8] = 1.0 - (2.0 * (a[0] + a[1] + a[2]+ a[3] + a[4] + a[5] + a[6]+ a[7]));
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    ah2[i] = ah[i] * 0.5;
  }
  
  /* Mètode de composició de tres salts (Suzuki-Yoshida) */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Bucle Störmer-Verlet */
    for (k = 0; k < s; k++) {
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += ah2[k] * gT;
	}
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= ah[k] * gV;
	}
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += ah2[k] * gT;
	}
      }
    }

    Neval += (3 * (planetes - 1));
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
