/* 28-06-2018 */
/* alex */
/* 18_pss613.c */
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
  int s = 13, m = 12;
  real a[s], ah[s];
  real g[m], gh[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficicients */
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
  for (i = 0; i < s; i++)
    ah[i] = a[i] * h;
  for (i = 0; i < m; i++)
    gh[i] = g[i] * h;

  /* preprocessat */
  t0 = temps();
  for (i = 0; i < m; i++)
    phi_storAdj(masses, q, p, planetes, gh[i]);
  Neval += (m * (planetes - 1));
  t += temps() - t0;

  /* Bucle principal */
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Bucle Störmer-Verlet */
    for (i = 0; i < s; i++)
      phi_storAdj(masses, q, p, planetes, ah[i]);
    Neval += (s * (planetes - 1));
    t += temps() - t0;

    /* imprimir */
    if ((it % pit) == 0) {
      /* copia per evitar el preprocessat */
      copiar(q, q_cop, planetes);
      copiar(p, p_cop, planetes);
      
      /* postprocessat */
      t0 = temps();
      for (i = m - 1; i >= 0; i--)
	phi_storAdj(masses, q, p, planetes, -gh[i]);
      Neval += (m * (planetes - 1));
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
