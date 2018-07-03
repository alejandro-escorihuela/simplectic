/* 28-06-2018 */
/* alex */
/* 19_ps44.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP], q_cop[MAX_PLA][COMP], p_cop[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s = 4;
  real a[s], ah[s];
  real b[s + 1], bh[s + 1];
  real y[s], yh[s];
  real z[s], zh[s];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[3] = (57.0 + ARREL_Q(18069.0)) / 300.0;
  a[1] = a[2] = 0.5 - a[0];
  b[0] = b[4] = 6.0 / 25.0;
  b[1] = b[3] = -0.1L;
  b[2] = 1.0 - (2.0 * (b[0] + b[1]));
  y[0] = -0.5903105192555323L;
  y[1] = 0.0013732794565115L;
  y[2] = 0.3958521503201655L;
  y[3] = -(y[0] + y[1] + y[2]);
  z[0] = -0.117183575320267L;
  z[1] = 0.8785444960116207L;
  z[2] = -0.8972532123604465L;
  z[3] = -(z[0] + z[1] + z[2]);
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }
  bh[s] = b[s] * h;
  
  /* preprocessat */
  t0 = temps();
  for (i = 0; i < s; i++) {
    phi_T(masses, q, p, planetes, zh[i]);
    phi_V(masses, q, p, planetes, yh[i]);
  }
  Neval += (s * (planetes - 1));
  t += temps() - t0;
  
  /* Bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Mètode */
    for (i = 0; i < s; i++) {
      phi_V(masses, q, p, planetes, bh[i]);
      phi_T(masses, q, p, planetes, ah[i]);
    }
    phi_V(masses, q, p, planetes, bh[s]);
    Neval += ((s + 1) * (planetes - 1));
    t += temps() - t0;

    /* imprimir */
    if ((it % pit) == 0) {
      /* copia per evitar el preprocessat */
      copiar(q, q_cop, planetes);
      copiar(p, p_cop, planetes);     
      /* postprocessat */
      t0 = temps();
      for (i = s - 1; i >= 0; i--) {
      	phi_V(masses, q, p, planetes, -yh[i]);
      	phi_T(masses, q, p, planetes, -zh[i]);
      }
      Neval += (s * (planetes - 1));
      t += temps() - t0;
      
      /* escriptura */
      H = energia(masses, q, p, planetes);
      DH = ABSOLUT(H - H0);
      if (DH > Hemax)
	Hemax = DH;      
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
      
      /* copia per evitar el preprocessat */
      copiar(q_cop, q, planetes);
      copiar(p_cop, p, planetes); 
    }
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
