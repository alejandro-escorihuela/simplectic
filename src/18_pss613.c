/* 28-06-2018 */
/* alex */
/* 18_pss613.c */
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
  int s = 13, m = 12;
  real a[s], ah[s], ah2[s];
  real g[m], gh[m], gh2[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  a[0] = a[1] = a[2] = a[3] = a[9] = a[10] = a[11] = a[12] = 0.1256962887201060L;
  a[4] = a[8] = 0.1480706601149650L;
  a[5] = a[7] = -0.3508563708238280L;
  a[6] = 1.0 - (2.0 * (a[0] + a[1] + a[2] + a[3] + a[4] + a[5]));
  g[0] = 0.1L;
  g[1] = 0.2250802987611760L;
  g[2] = 0.1912446945111610L;
  g[3] = -0.2127637921948900L;
  g[4] = -0.0966015730658229L;
  g[5] = -(g[0] + g[1] + g[2] + g[3] + g[4]);
  for (i = 6; i < m; i++)
    g[i] = -g[i - 6]; 
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    ah2[i] = ah[i] * 0.5;
  }
  for (i = 0; i < m; i++) {
    gh[i] = g[i] * h;
    gh2[i] = gh[i] * 0.5;
  } 
  /* Mètode de composició de tres salts (Suzuki-Yoshida) */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = 0; k < m; k++) {
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += gh2[k] * gT;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= gh[k] * gV;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += gh2[k] * gT;
	  }
	}
      }
      Neval += (m * (planetes - 1));
    }
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
    Neval += (s * (planetes - 1));
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = m - 1; k >= 0; k--) {
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += -gh2[k] * gT;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= -gh[k] * gV;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += -gh2[k] * gT;
	  }
	}
      }
      Neval += (m * (planetes - 1));
    }
   
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
