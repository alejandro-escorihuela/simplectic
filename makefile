# 13-04-2018
# cosmogat

EXE = ss69 tjc4 rkg4 rkn4 rk-4 llib stor simp expl
COM = gcc # Compilador
LIB = -lm -lquadmath # Llibreries (-l*, -L*, -I*)
MAC = -D_GNU_SOURCE # Macros (-D*)
AVS = -W -Wall -Wextra -ansi -pedantic # Avisos
OPT = -march=native -O2 -pipe # Optimitzacio
DEP = -g -DDEBUG # Depuracio, no recomanable junt amb $(OPT)
OPC = $(DEP) $(AVS) $(MAC) -std=c11 # Opcions
DIR = /usr/local/bin # Directori per a instalar

all: $(EXE)

ss69: src/ss69.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

tjc4: src/tjc4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

rkg4: src/rkg4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

rkn4: src/rkn4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

rk-4: src/rk-4.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

llib: src/llib.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

stor: src/stor.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

simp: src/simp.c solar.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

expl: src/expl.c solar.o
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
