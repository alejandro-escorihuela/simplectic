/* 28-06-2018 */
/* alex */
/* 20_ps44s.c */
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
  int s = 4;
  int m = 3;
  real a[2 * s], ah[2 * s];
  real g[(2 * m) + 1], gh[(2 * m) + 1];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  a[0] = a[7] =  6.0 / 25.0;
  a[1] = a[6] =  9.0 / 25.0;
  a[2] = a[5] =  (-15.0 + ARREL_Q(18069)) / 300.0;
  a[3] = a[4] = (-15.0 - ARREL_Q(18069)) / 300.0;
  g[0] = 0.1171835753202670L;
  g[1] = 0.4731269439352650L;
  g[2] = -1.351671439946886L;
  g[3] = 1.3502981604903750L;
  g[4] = 0.4530449481299280L;
  g[5] = 0.0571927978097620L;
  g[6] = -(g[0] + g[1] + g[2] + g[3] + g[4] + g[5]);
  for (i = 0; i < (2 * s); i++)
    ah[i] = a[i] * h;
  for (i = 0; i < (2 * m) + 1; i++)
    gh[i] = g[i] * h;
  
  /* Mètode d'escissió */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = 0; k < m; k++) {
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += gh[2 * k] * gT;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= gh[2 * k] * gV;
	  }
	}  	
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= gh[(2 * k) + 1] * gV;
	  }
	} 
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += gh[(2 * k) + 1] * gT;
	  }
	}   
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += gh[2 * m] * gT;
	}
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= gh[2 * m] * gV;
	}
      }     
      Neval += (((2 * m) + 1) * (planetes - 1));
    }
    /* Bucle per a k */
    for (k = 0; k < s; k++) {
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= ah[2 * k] * gV;
	}
      } 
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += ah[2 * k] * gT;
	}
      }   
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += ah[(2 * k) + 1] * gT;
	}
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= ah[(2 * k) + 1] * gV;
	}
      }  
    }
    Neval += ((2 * s) * (planetes - 1));
    
    /* preprocessat */
    if ((it % pit) == 0) {
      for (k = m; k >= 1; k--) {
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= -gh[2 * k] * gV;
	  }
	} 
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += -gh[2 * k] * gT;
	  }
	}   
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gT = (p[i][j] / masses[i]);
	    q[i][j] += -gh[(2 * k) - 1] * gT;
	  }
	}
	for (i = 1; i < planetes; i++) {
	  for (j = 0; j < COMP; j++) {
	    gV = gradV(masses, q, i, j, planetes);
	    p[i][j] -= -gh[(2 * k) - 1] * gV;
	  }
	}  
      }
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gV = gradV(masses, q, i, j, planetes);
	  p[i][j] -= -gh[0] * gV;
	}
      } 
      for (i = 1; i < planetes; i++) {
	for (j = 0; j < COMP; j++) {
	  gT = (p[i][j] / masses[i]);
	  q[i][j] += -gh[0] * gT;
	}
      }       
      Neval += (((2 * m) + 1) * (planetes - 1));
    }

    t += temps() - t0;
    if ((it % pit) == 0) {
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
