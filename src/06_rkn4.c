/* 25-04-2018 */
/* alex */
/* 06_rkn4.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, j, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA];
  real q[MAX_PLA][COMP], p[MAX_PLA][COMP], v[MAX_PLA][COMP];
  real l1[MAX_PLA][COMP], l2[MAX_PLA][COMP], l3[MAX_PLA][COMP], Y1[MAX_PLA][COMP], Y2[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h, h2;
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  h2 = h * h;
  for (i = 1; i < planetes; i++)
    for (j = 0; j < COMP; j++)
      v[i][j] = p[i][j] / masses[i];
  
  /* Mètode Runge-Kutta-Nyström */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	l1[i][j] = deriv2q(masses, q, i, j, planetes);
	Y1[i][j] = q[i][j] + (0.5 * h * v[i][j]) + (0.125 * h2 * l1[i][j]);
      }
    }
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {
	l2[i][j] = deriv2q(masses, Y1, i, j, planetes);
	Y2[i][j] = q[i][j] + (h * v[i][j]) + (0.5 * h2 * l2[i][j]);
      }
    }
    for (i = 1; i < planetes; i++)
      for (j = 0; j < COMP; j++)
	l3[i][j] = deriv2q(masses, Y2, i, j, planetes);
	
    for (i = 1; i < planetes; i++) {
      for (j = 0; j < COMP; j++) {	
	q[i][j] += (h * v[i][j]) + ((h2 / 6.0) * (l1[i][j] + (2.0 * l2[i][j])));
	v[i][j] += ((h / 6.0) * (l1[i][j] + (4.0 * l2[i][j]) + l3[i][j]));
	p[i][j] = v[i][j] * masses[i];
      }
      Neval += 3;
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
