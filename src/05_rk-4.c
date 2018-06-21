/* 23-04-2018 */
/* alex */
/* 05_rk-4.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA];
  real q[MAX_PLA][COMP], p[MAX_PLA][COMP];
  real k1q[MAX_PLA][COMP], k2q[MAX_PLA][COMP], k3q[MAX_PLA][COMP], k4q[MAX_PLA][COMP], k1p[MAX_PLA][COMP], k2p[MAX_PLA][COMP], k3p[MAX_PLA][COMP], k4p[MAX_PLA][COMP];
  real Y1q[MAX_PLA][COMP], Y2q[MAX_PLA][COMP], Y3q[MAX_PLA][COMP], Y1p[MAX_PLA][COMP], Y2p[MAX_PLA][COMP], Y3p[MAX_PLA][COMP];
  /* int k; */
  /* real x[COORD][MAX_PLA][COMP]; */
  /* real k1[COORD][MAX_PLA][COMP], k2[COORD][MAX_PLA][COMP], k3[COORD][MAX_PLA][COMP], k4[COORD][MAX_PLA][COMP]; */
  /* real Y1[COORD][MAX_PLA][COMP], Y2[COORD][MAX_PLA][COMP], Y3[COORD][MAX_PLA][COMP]; */
  real H0, H, DH, Hemax = 0.0;
  real h;
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  /* planetes = carregar_planetes(f_ini, masses, noms, x[0], x[1]); */
  H0 = energia(masses, q, p, planetes);
  /* H0 = energia(masses, x[0], x[1], planetes); */
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);
  
  /* Mètode Runge-Kutta 4 */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
    	k1q[i][j] = p[i][j] / masses[i];
    	k1p[i][j] = -gradV(masses, q, i, j, planetes);
    	Y1p[i][j] = p[i][j] + ((h * k1p[i][j]) / 2.0);
    	Y1q[i][j] = q[i][j] + ((h * k1q[i][j]) / 2.0);
      }
    }
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
    	k2q[i][j] = Y1p[i][j] / masses[i];
    	k2p[i][j] = -gradV(masses, Y1q, i, j, planetes);
    	Y2p[i][j] = p[i][j] + ((h * k2p[i][j]) / 2.0);
    	Y2q[i][j] = q[i][j] + ((h * k2q[i][j]) / 2.0);
      }
    }
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
    	k3q[i][j] = Y2p[i][j] / masses[i];
    	k3p[i][j] = -gradV(masses, Y2q, i, j, planetes);
    	Y3p[i][j] = p[i][j] + (h * k3p[i][j]);
    	Y3q[i][j] = q[i][j] + (h * k3q[i][j]);
      }
    }
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
    	k4q[i][j] = Y3p[i][j] / masses[i];
    	k4p[i][j] = -gradV(masses, Y3q, i, j, planetes);
      }
    }
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
    	q[i][j] = q[i][j] + ((h * (k1q[i][j] + (2.0 * k2q[i][j]) + (2.0 * k3q[i][j]) + k4q[i][j])) / 6.0);
    	p[i][j] = p[i][j] + ((h * (k1p[i][j] + (2.0 * k2p[i][j]) + (2.0 * k3p[i][j]) + k4p[i][j])) / 6.0);
      }
      Neval += 4;
    }

    /* for (i = 1; i < planetes; i++) { */
    /*   for (j = 0; j < COMP; j++) { */
    /* 	for (k = 0; k < COORD; k++) { */
    /* 	  k1[k][i][j] = deriv(k, masses, x, i, j, planetes); */
    /* 	  Y1[k][i][j] = x[k][i][j] + ((h * k1[k][i][j]) / 2.0); */
    /* 	} */
    /*   } */
    /* } */
    /* for (i = 1; i < planetes; i++) { */
    /*   for (j = 0; j < COMP; j++) { */
    /* 	for (k = 0; k < COORD; k++) { */
    /* 	  k2[k][i][j] = deriv(k, masses, Y1, i, j, planetes); */
    /* 	  Y2[k][i][j] = x[k][i][j] + ((h * k2[k][i][j]) / 2.0); */
    /* 	} */
    /*   } */
    /* } */
    /* for (i = 1; i < planetes; i++) { */
    /*   for (j = 0; j < COMP; j++) { */
    /* 	for (k = 0; k < COORD; k++) { */
    /* 	  k3[k][i][j] = deriv(k, masses, Y2, i, j, planetes); */
    /* 	  Y3[k][i][j] = x[k][i][j] + (h * k3[k][i][j]); */
    /* 	} */
    /*   } */
    /* } */
    /* for (i = 1; i < planetes; i++) { */
    /*   for (j = 0; j < COMP; j++) { */
    /* 	for (k = 0; k < COORD; k++) { */
    /* 	  k4[k][i][j] = deriv(k, masses, Y3, i, j, planetes); */
    /* 	} */
    /*   } */
    /* } */
    /* for (i = 1; i < planetes; i++) { */
    /*   for (j = 0; j < COMP; j++) { */
    /* 	for (k = 0; k < COORD; k++) */
    /* 	  x[k][i][j] = x[k][i][j] + ((h * (k1[k][i][j] + (2.0 * k2[k][i][j]) + (2.0 * k3[k][i][j]) + k4[k][i][j])) / 6.0); */
    /*   } */
    /*   Neval += 4; */
    /* } */
    
    t += temps() - t0;
    H = energia(masses, q, p, planetes);
    /* H = energia(masses, x[0], x[1], planetes); */
    DH = ABSOLUT(H - H0);
    if (DH > Hemax)
      Hemax = DH;
    if ((it % pit) == 0) {
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
      /* escriure_fitxers(fit_pl, pop, ((real) it) * h, x[0], x[1], H0, H, planetes); */
    }
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
