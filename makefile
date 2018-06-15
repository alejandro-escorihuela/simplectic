# 13-04-2018
# cosmogat

EXE = 11_ss817 10_ss69 09_ss45 08_tjc4 07_rkg4 06_rkn4 05_rk-4 04_llib 03_stor 02_simp 01_expl
COM = gcc # Compilador
LIB = -lm -lquadmath # Llibreries (-l*, -L*, -I*)
MAC = -D_GNU_SOURCE # Macros (-D*)
AVS = -W -Wall -Wextra -ansi -pedantic # Avisos
OPT = -march=native -O2 -pipe # Optimitzacio
DEP = -g -DDEBUG # Depuracio, no recomanable junt amb $(OPT)
OPC = $(DEP) $(AVS) $(MAC) -std=c11 # Opcions
DIR = /usr/local/bin # Directori per a instalar

all: $(EXE)

11_ss817: src/11_ss817.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

10_ss69: src/10_ss69.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

09_ss45: src/09_ss45.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

08_tjc4: src/08_tjc4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

07_rkg4: src/07_rkg4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

06_rkn4: src/06_rkn4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

05_rk-4: src/05_rk-4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

04_llib: src/04_llib.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

03_stor: src/03_stor.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

02_simp: src/02_simp.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

01_expl: src/01_expl.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

solar.o: src/solar.c src/solar.h
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

clean:
	@echo "\033[0;33m"Netejant..."\033[0m"
	@echo -n "  \033[0;37m"
	rm -rf *.o *~
	@echo -n "\033[0m"

clean_exe:
	@echo "\033[0;33m"Netejant executables"\033[0m"
	@echo -n "  \033[0;37m"
	rm -rf $(EXE)
	@echo -n "\033[0m"

clean_data:
	@echo "\033[0;31m"Netejant dades"\033[0m"
	@echo -n "  \033[0;37m"
	rm -rf ./dat
	@echo -n "\033[0m"

clean_all: clean clean_exe clean_data
