/* 07-11-2018 */
/* alex */
/* composicio.c */
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "metodes.h"
#include "solar.h"
#include "molecular.h"
#include "fput.h"

int main (int num_arg, char * vec_arg[]) {
  int i, it, Npart, N, pop, pit, Neval = 0;
  char noms[MAX_PAR][MAX_CAD], f_ini[20], f_coef[20], t_metode[20], t_poten[20];
  real masses[MAX_PAR], q[MAX_PAR][COMP], p[MAX_PAR][COMP], v[MAX_PAR][COMP];
  real a[NUM_MAX_COEF], b[NUM_MAX_COEF], y[NUM_MAX_COEF], z[NUM_MAX_COEF];
  real ah[NUM_MAX_COEF], bh[NUM_MAX_COEF], yh[NUM_MAX_COEF], zh[NUM_MAX_COEF];
  int tam_a = 0, tam_b = 0, tam_y = 0, tam_z = 0;
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s;
  double t0, t = 0.0;
  FILE * fit_pl[MAX_PAR + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini, t_poten, t_metode, f_coef);

  /* tipus de potencial */
  if (strcmp(t_poten, "solar") == 0) {
    gradV = gradVSolar;
    egradV = egradVSolar;
    deriv2q = deriv2qSolar;
    gradVmod = gradVmodSolar;
    phi0 = phiKepler;
    q_conservada = energiaSolar;
    Npart = init_planetes(f_ini, masses, noms, q, p);
  }
  else if (strcmp(t_poten, "molecular") == 0) {
    gradV = gradVMolecular;
    egradV = egradVMolecular;
    deriv2q = deriv2qMolecular;
    gradVmod = gradVmodMolecular;
    phi0 = phi0Molecular;
    q_conservada = energiaMolecular;
    Npart = init_molecules(masses, noms, q, p);
  }
  else if (strcmp(t_poten, "fput") == 0) {
    gradV = gradVFPUT;
    egradV = egradVFPUT;
    deriv2q = deriv2qFPUT;
    gradVmod = gradVmodFPUT;
    phi0 = phi0FPUT;
    q_conservada = energiaFPUT;
    Npart = init_FPUT(masses, noms, q, p);
  }
  else {
    fputs("El potencial especificat no existeix\n", stderr);
    exit(1);
  }
  
  H0 = q_conservada(masses, q, p, Npart);
  obrir_fitxers(fit_pl, noms, f_ini, t_poten, f_coef, Npart);
  lectura_coef(f_coef, a, b, y, z, &tam_a, &tam_b, &tam_y, &tam_z);
  for (i = 0; i < NUM_MAX_COEF; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
  }
  (void) yh;
  (void) zh;
  
  /* preconfiguració per a cada mètode */
  if (strcmp(t_metode, "ss") == 0)
    s = tam_a;
  else if (strcmp(t_metode, "sb") == 0)
    s = tam_a;
  else if (strcmp(t_metode, "sa") == 0)
    s = tam_b;
  else if (strcmp(t_metode, "sx") == 0)
    s = tam_a;
  else if (strcmp(t_metode, "nb") == 0) {
    p2v(masses, p, v, Npart);
    s = tam_a;
  }
  else if (strcmp(t_metode, "na") == 0) {
    p2v(masses, p, v, Npart);
    s = tam_b;
  }
  else if (strcmp(t_metode, "nia") == 0)
    s = tam_b;
  else {
    fputs("El mètode especificat no existeix\n", stderr);
    exit(1);
  }
  
  /* bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* tipus de mètode */
    if (strcmp(t_metode, "ss") == 0) {
      for (i = 0; i < s; i++)
	phi_storAdj(masses, q, p, Npart, ah[i]);
      Neval += (s * Npart);
    }
    else if (strcmp(t_metode, "sb") == 0) {
      for (i = 0; i < s; i++) {
	phi_V(masses, q, p, Npart, bh[i]);
	phi_T(masses, q, p, Npart, ah[i]);
      }
      phi_V(masses, q, p, Npart, bh[s]);
      Neval += ((s + 1) * Npart);
    }
    else if (strcmp(t_metode, "sa") == 0) {
      for (i = 0; i < s; i++) {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_V(masses, q, p, Npart, bh[i]);
      }
      phi_T(masses, q, p, Npart, ah[s]);
      Neval += (s * Npart);
    }
    else if (strcmp(t_metode, "sx") == 0) {
      for (i = 0; i < s; i += 2) {
	phi_simpVT(masses, q, p, Npart, ah[i]);
	phi_simpTV(masses, q, p, Npart, ah[i + 1]);
      }
      Neval += (s * Npart);
    }    
    else if (strcmp(t_metode, "nb") == 0) {
      for (i = 0; i < s; i++) {
	phi_Vv(masses, q, v, Npart, bh[i]);
	phi_Tv(masses, q, v, Npart, ah[i]);
      }
      phi_Vv(masses, q, v, Npart, bh[s]);
      v2p(masses, p, v, Npart);
      Neval += ((s + 1) * Npart);
    }
    else if (strcmp(t_metode, "na") == 0) {
      for (i = 0; i < s; i++) {
	phi_Tv(masses, q, v, Npart, ah[i]);
	phi_Vv(masses, q, v, Npart, bh[i]);
      }
      phi_Tv(masses, q, v, Npart, ah[s]);
      v2p(masses, p, v, Npart);
      Neval += (s * Npart);
    }
    else if (strcmp(t_metode, "nia") == 0) {
      for (i = 0; i < s; i++) {
	phi_H0(masses, q, p, Npart, ah[i]);
	phi_eV1(masses, q, p, Npart, bh[i]);
      }
      phi_H0(masses, q, p, Npart, ah[s]);    
      Neval += (s * Npart);
    }

    t += temps() - t0;
    H = q_conservada(masses, q, p, Npart);
    DH = ABSOLUT(H - H0);
    if (DH > Hemax)
      Hemax = DH;
    if ((it % pit) == 0)
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, Npart);
  }
  tancar_fitxers(fit_pl, Npart);
  print_info(h, t, Neval, Hemax / H0);
  return 0;
}

