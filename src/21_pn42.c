/* 30-06-2018 */
/* alex */
/* 21_pn42.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solar.h"

int main (int num_arg, char * vec_arg[]){
  int i, it, planetes, N, pop, pit, Neval = 0;
  char noms[MAX_PLA][MAX_CAD], f_ini[20];
  real masses[MAX_PLA], q[MAX_PLA][COMP], p[MAX_PLA][COMP], v[MAX_PLA][COMP], q_cop[MAX_PLA][COMP], v_cop[MAX_PLA][COMP];
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s = 2;
  int m = 3;
  real a[s], ah[s];
  real b[s], bh[s];
  real y[m], yh[m];
  real z[m], zh[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = -0.1L;
  a[1] = 1.0 - a[0];
  b[0] = 0.5 - ARREL_Q(133.0 / 132.0);
  b[1] = 1.0 - b[0];
  y[0] = -0.1937696215758170L;
  y[1] = -0.931151146256426L;
  y[2] = 0.1053624334726687L;
  z[0] = -0.5349755290809216L;
  z[1] = 0.3086327690445878L;
  z[2] = -0.142837501141108L;
  for (i = 0; i < s; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
  }
  for (i = 0; i < m; i++) {
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }  
  /* p -> v */
  p2v(masses, p, v, planetes);
  
  /* preprocessat */
  t0 = temps();
  for (i = 0; i < m; i++) {
    phi_Vv(masses, q, v, planetes, yh[i]);
    phi_Tv(masses, q, v, planetes, zh[i]);
  }
  Neval += (m * (planetes - 1));
  t += temps() - t0;
  
  /* Bucle principal */
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Mètode */
    for (i = 0; i < s; i++) {
      phi_Vv(masses, q, v, planetes, bh[i]);
      phi_Tv(masses, q, v, planetes, ah[i]);
    }
    Neval += (s * (planetes - 1));
    t += temps() - t0;
    /* imprimir */
    if ((it % pit) == 0) {
      /* copia per evitar el preprocessat */
      copiar(q, q_cop, planetes);
      copiar(v, v_cop, planetes);  
      /* postprocessat */
      t0 = temps();
      for (i = m - 1; i >= 0; i--) {
	phi_Tv(masses, q, v, planetes, -zh[i]);
	phi_Vv(masses, q, v, planetes, -yh[i]);
      }      
      Neval += (m * (planetes - 1));
      t += temps() - t0;     
      /* escriptura */
      /* v -> p */
      v2p(masses, p, v, planetes);
      H = energia(masses, q, p, planetes);
      DH = ABSOLUT(H - H0);
      if (DH > Hemax)
	Hemax = DH;      
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, planetes);
      /* copia per evitar el preprocessat */
      copiar(q_cop, q, planetes);
      copiar(v_cop, v, planetes);    
    }
  }
  tancar_fitxers(fit_pl, planetes);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}
