/* 07-11-2018 */
/* alex */
/* simplectic.c */
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "metodes.h"
#include "kepler.h"
#include "solar.h"
#include "molecular.h"
#include "fput.h"
#include "harmonic.h"

int main (int num_arg, char * vec_arg[]) {
  int i, it, Npart, N, pop, pit, Neval = 0, Neval_Vmod = 0;
  char noms[MAX_PAR][MAX_CAD], f_ini[20], f_coef[20], t_metode[20], t_nucli[20], t_poten[20];
  real masses[MAX_PAR], q[MAX_PAR][COMP], p[MAX_PAR][COMP], q_cop[MAX_PAR][COMP], p_cop[MAX_PAR][COMP];
  real a[NUM_MAX_COEF], b[NUM_MAX_COEF], y[NUM_MAX_COEF], z[NUM_MAX_COEF], g[NUM_MAX_COEF], bm;
  real ah[NUM_MAX_COEF], bh[NUM_MAX_COEF], yh[NUM_MAX_COEF], zh[NUM_MAX_COEF], gh[NUM_MAX_COEF], bmh;
  int tam_a = 0, tam_b = 0, tam_y = 0, tam_z = 0, tam_g = 0;
  real H0, H, DH, Hemax = 0.0;
  real h;
  int s, sm, m;
  double t0, t = 0.0, fac_Vmod;
  FILE * fit_pl[MAX_PAR + 1];

  carregar_configuracio(num_arg, vec_arg, &h, &N, &pop, &pit, f_ini, t_poten, t_metode, f_coef);
  
  /* tipus de potencial */
  if (strcmp(t_poten, "kepler") == 0) {
    gradV = gradVKepler;
    egradV = egradVKepler;
    deriv2q = deriv2qKepler;
    gradVmod = gradVmodKepler;
    phi0 = phi0Kepler;
    q_conservada = energiaKepler;
    Npart = init_kepler(masses, noms, q, p);
    fac_Vmod = 0.11;
  }
  else if (strcmp(t_poten, "solar") == 0) {
    gradV = gradVSolar;
    egradV = egradVSolar;
    deriv2q = deriv2qSolar;
    gradVmod = gradVmodSolar;
    phi0 = phiKepler;
    q_conservada = energiaSolar;
    Npart = init_planetes(f_ini, masses, noms, q, p);
    fac_Vmod = 0.08;
  }
  else if (strcmp(t_poten, "molecular") == 0) {
    gradV = gradVMolecular;
    egradV = egradVMolecular;
    deriv2q = deriv2qMolecular;
    gradVmod = gradVmodMolecular;
    phi0 = phi0Molecular;
    q_conservada = energiaMolecular;
    Npart = init_molecules(masses, noms, q, p);
    fac_Vmod = 0.20;
  }
  else if (strcmp(t_poten, "fput") == 0) {
    gradV = gradVFPUT;
    egradV = egradVFPUT;
    deriv2q = deriv2qFPUT;
    gradVmod = gradVmodFPUT;
    phi0 = phi0FPUT;
    q_conservada = energiaFPUT;
    Npart = init_FPUT(masses, noms, q, p);
    fac_Vmod = 0.10;
  }
  else if (strcmp(t_poten, "harmonic") == 0) {
    gradV = gradVharmonic;
    egradV = egradVharmonic;
    deriv2q = deriv2qharmonic;
    gradVmod = gradVmodharmonic;
    phi0 = phi0harmonic;
    q_conservada = energiaharmonic;
    Npart = init_harmonic(masses, noms, q, p);
    fac_Vmod = 0.05;
  }
  else {
    fputs("El potencial especificat no existeix\n", stderr);
    exit(1);
  }
  
  H0 = q_conservada(masses, q, p, Npart);
  obrir_fitxers(fit_pl, noms, f_ini, t_poten, f_coef, Npart);
  lectura_coef(f_coef, a, b, y, z, g, &bm, &tam_a, &tam_b, &tam_y, &tam_z, &tam_g);
  for (i = 0; i < NUM_MAX_COEF; i++) {
    ah[i] = a[i] * h;
    bh[i] = b[i] * h;
    yh[i] = y[i] * h;
    zh[i] = z[i] * h;
    gh[i] = g[i] * h;
  }
  bmh = bm * h * h * h;
  
  /* preconfiguració per a cada mètode */
  if (t_metode[0] != 'p')
    strcpy(t_nucli, t_metode);
  else {
    memcpy(t_nucli, &t_metode[1], strlen(t_metode) - 1);
    t_nucli[strlen(t_metode) - 1] = '\0';
  }

  m = tam_z;
  if ((strcmp(t_metode, "pss") == 0) || (strcmp(t_metode, "psx") == 0)) {
      m = tam_g;
    }
  if (strcmp(t_nucli, "ss") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "sb") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "sa") == 0)
    s = tam_b;
  else if (strcmp(t_nucli, "sx") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "ma") == 0)
    s = tam_b;
  else if (strcmp(t_nucli, "mb") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "nb") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "na") == 0)
    s = tam_b;
  else if (strcmp(t_nucli, "n") == 0)
    s = tam_a;
  else if (strcmp(t_nucli, "nia") == 0)
    s = tam_b;
  else {
    fputs("El mètode especificat no existeix\n", stderr);
    exit(1);
  }

  /* preprocessat */
  if (t_metode[0] == 'p') {
    t0 = temps();
    if (strcmp(t_metode, "pss") == 0) {
      for (i = 0; i < m; i++)
	phi_storAdj(masses, q, p, Npart, gh[i]);
    }
    else if (strcmp(t_metode, "psx") == 0) {
      for (i = 0; i < m - 1; i += 2) {
	phi_simpTV(masses, q, p, Npart, gh[i]);
	phi_simpVT(masses, q, p, Npart, gh[i + 1]);
      }
      phi_simpTV(masses, q, p, Npart, gh[m - 1]);
    }
    else if (strcmp(t_metode, "psb") == 0) {
       for (i = 0; i < m; i++) {
	phi_T(masses, q, p, Npart, zh[i]);
	phi_V(masses, q, p, Npart, yh[i]);    
      }     
    }
    else if (strcmp(t_metode, "pnia") == 0) {
      for (i = 0; i < m; i++) {
	phi_H0(masses, q, p, Npart, zh[i]);
	phi_eV1(masses, q, p, Npart, yh[i]);
      }
    }
    else {
      for (i = 0; i < m; i++) {
	phi_V(masses, q, p, Npart, yh[i]);
	phi_T(masses, q, p, Npart, zh[i]);    
      }
    }
    Neval += (m * Npart);
    t += temps() - t0;
  }
  /* bucle principal */  
  for (it = 0; it < N; it++) {
    t0 = temps();
    /* tipus de mètode */
    if (strcmp(t_nucli, "ss") == 0) {
      for (i = 0; i < s; i++)
	phi_storAdj(masses, q, p, Npart, ah[i]);
      Neval += (s * Npart);
    }
    else if (strcmp(t_nucli, "sa") == 0) {
      for (i = 0; i < s; i++) {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_V(masses, q, p, Npart, bh[i]);
      }
      phi_T(masses, q, p, Npart, ah[s]);
      Neval += (s * Npart);
    }
    else if (strcmp(t_nucli, "sb") == 0) {
      for (i = 0; i < s; i++) {
	phi_V(masses, q, p, Npart, bh[i]);
	phi_T(masses, q, p, Npart, ah[i]);
      }
      phi_V(masses, q, p, Npart, bh[s]);
      Neval += ((s + 1) * Npart);
    }
    else if (strcmp(t_nucli, "sx") == 0) {
      for (i = 0; i < s; i += 2) {
	phi_simpVT(masses, q, p, Npart, ah[i]);
	phi_simpTV(masses, q, p, Npart, ah[i + 1]);
      }
      Neval += (s * Npart);
    }
    else if (strcmp(t_nucli, "ma") == 0) {
      sm = (s - 1) / 2;
      i = 0;
      while (i < sm) {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_V(masses, q, p, Npart, bh[i]);
	i++;
      }
      if ((s % 2) == 0 ) {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_Vm(masses, q, p, Npart, bh[i], bmh);
	i++;
	phi_T(masses, q, p, Npart, ah[i]);
	phi_Vm(masses, q, p, Npart, bh[i], bmh);
	i++;
	phi_T(masses, q, p, Npart, ah[i]);
	Neval_Vmod += (2 * Npart);
      }
      else {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_Vm(masses, q, p, Npart, bh[i], bmh);
	i++;
	phi_T(masses, q, p, Npart, ah[i]);
	Neval_Vmod += Npart;
      }
      while (i < s) {
	phi_V(masses, q, p, Npart, bh[i]);
	i++;
	phi_T(masses, q, p, Npart, ah[i]);
      }
      Neval += (s * Npart);      
    }
    else if (strcmp(t_nucli, "mb") == 0) {
      sm = s / 2;
      i = 0;
      while (i < sm) {
      	phi_V(masses, q, p, Npart, bh[i]);
      	phi_T(masses, q, p, Npart, ah[i]);
      	i++;
      }
      if ((s % 2) == 0) {
      	phi_Vm(masses, q, p, Npart, bh[i], bmh);
      	Neval_Vmod += Npart;
      }
      else {
      	phi_Vm(masses, q, p, Npart, bh[i], bmh);
      	phi_T(masses, q, p, Npart, ah[i]);
      	i++;
      	phi_Vm(masses, q, p, Npart, bh[i], bmh);
      	Neval_Vmod += (2 * Npart);
      }
      while (i < s) {
      	phi_T(masses, q, p, Npart, ah[i]);
      	i++;
      	phi_V(masses, q, p, Npart, bh[i]);
      }
      Neval += ((s + 1) * Npart);
    }
    else if (strcmp(t_nucli, "na") == 0) {
      for (i = 0; i < s; i++) {
	phi_T(masses, q, p, Npart, ah[i]);
	phi_V(masses, q, p, Npart, bh[i]);
      }
      phi_T(masses, q, p, Npart, ah[s]);
      Neval += (s * Npart);
    }
    else if (strcmp(t_nucli, "nb") == 0) {
      for (i = 0; i < s; i++) {
      	phi_V(masses, q, p, Npart, bh[i]);
      	phi_T(masses, q, p, Npart, ah[i]);
      }
      phi_V(masses, q, p, Npart, bh[s]);
      Neval += ((s + 1) * Npart);
    }
    else if (strcmp(t_nucli, "n") == 0) {
      for (i = 0; i < s; i++) {
      	phi_V(masses, q, p, Npart, bh[i]);
      	phi_T(masses, q, p, Npart, ah[i]);
      }
      Neval += (s * Npart);
    }    
    else if (strcmp(t_nucli, "nia") == 0) {
      for (i = 0; i < s; i++) {
	phi_H0(masses, q, p, Npart, ah[i]);
	phi_eV1(masses, q, p, Npart, bh[i]);
      }
      phi_H0(masses, q, p, Npart, ah[s]);    
      Neval += (s * Npart);
    }

    t += temps() - t0;
    if ((it % pit) == 0) {
      if (t_metode[0] == 'p') {
	copiar(q, q_cop, Npart);
	copiar(p, p_cop, Npart);
	t0 = temps();
	if (strcmp(t_metode, "pss") == 0) {
	  for (i = m - 1; i >= 0; i--)
	    phi_storAdj(masses, q, p, Npart, -gh[i]);
	}
	else if (strcmp(t_metode, "psx") == 0) {
	  for (i = m - 1; i > 0; i -= 2) {
	    phi_simpVT(masses, q, p, Npart, -gh[i]);
	    phi_simpTV(masses, q, p, Npart, -gh[i - 1]);
	  }
	  phi_simpVT(masses, q, p, Npart, -gh[0]);
	}
	else if (strcmp(t_metode, "psb") == 0) {
	  for (i = m - 1; i >= 0; i--) {
	    phi_V(masses, q, p, Npart, -yh[i]);
	    phi_T(masses, q, p, Npart, -zh[i]);
	  }	  
	}
	else if (strcmp(t_metode, "pnia") == 0) {
	  for (i = m - 1; i >= 0; i--) {
	    phi_eV1(masses, q, p, Npart, -yh[i]);
	    phi_H0(masses, q, p, Npart, -zh[i]);
	  }
	}
	else {
	  for (i = m - 1; i >= 0; i--) {
	    phi_T(masses, q, p, Npart, -zh[i]);
	    phi_V(masses, q, p, Npart, -yh[i]);
	  }
	}
	Neval += (m * Npart);
	t += temps() - t0;
      }
      H = q_conservada(masses, q, p, Npart);
      DH = ABSOLUT(H - H0);
      if (DH > Hemax)
	Hemax = DH;
      escriure_fitxers(fit_pl, pop, ((real) it) * h, q, p, H0, H, Npart);
      if (((it % pit) == 0) && (t_metode[0] == 'p')) {
	copiar(q_cop, q, Npart);
	copiar(p_cop, p, Npart);
      }
    }
  }
  tancar_fitxers(fit_pl, Npart);
  print_info(h, t, Neval + ceil(((double) Neval_Vmod) * fac_Vmod), Hemax / H0);
  return 0;
}

