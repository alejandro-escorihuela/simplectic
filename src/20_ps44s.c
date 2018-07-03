/* 28-06-2018 */
/* alex */
/* 20_ps44s.c */
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
  int s = 8;
  int m = 7;
  real a[s], ah[s];
  real g[m], gh[m];
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PLA + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini);
  planetes = carregar_planetes(f_ini, masses, noms, q, p);
  H0 = energia(masses, q, p, planetes);
  obrir_fitxers(fit_pl, noms, f_ini, vec_arg[0], planetes);

  /* coeficients */
  a[0] = a[7] =  6.0 / 25.0;
  a[1] = a[6] =  (-15.0 + ARREL_Q(18069.0)) / 300.0;
  a[2] = a[5] = (-15.0 - ARREL_Q(18069.0)) / 300.0;
  a[3] = a[4] =  9.0 / 25.0;
  g[0] = 0.1171835753202670L;
  g[1] = 0.4731269439352650L;
  g[2] = -1.351671439946886L;
  g[3] = 1.3502981604903750L;
  g[4] = -0.4530449481299280L;
  g[5] = 0.0571927978097620L;
  g[6] = -(g[0] + g[1] + g[2] + g[3] + g[4] + g[5]);

  for (i = 0; i < s; i++)
    ah[i] = a[i] * h;
  for (i = 0; i < m; i++)
    gh[i] = g[i] * h;

  /* preprocessat */
  t0 = temps();
  for (i = 0; i < m - 1; i += 2) {
    phi_simpTV(masses, q, p, planetes, gh[i]);
    phi_simpVT(masses, q, p, planetes, gh[i + 1]);
  }
  phi_simpTV(masses, q, p, planetes, gh[m - 1]);
  Neval += (m * (planetes - 1));
  t += temps() - t0;
  
  /* Bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* Nucli */
    for (i = 0; i < s; i += 2) {
      phi_simpVT(masses, q, p, planetes, ah[i]);
      phi_simpTV(masses, q, p, planetes, ah[i + 1]);
    }
    Neval += (s * (planetes - 1));
    t += temps() - t0;

    /* imprimir */
    if ((it % pit) == 0) {
      /* copia per evitar el preprocessat */
      copiar(q, q_cop, planetes);
      copiar(p, p_cop, planetes);    
      /* postprocessat */
      t0 = temps();
      for (i = m - 1; i > 0; i -= 2) {
      	phi_simpVT(masses, q, p, planetes, -gh[i]);
      	phi_simpTV(masses, q, p, planetes, -gh[i - 1]);
      }
      phi_simpVT(masses, q, p, planetes, -gh[0]);
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
