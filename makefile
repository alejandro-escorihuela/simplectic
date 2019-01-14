# 13-04-2018
# cosmogat

EXE = composicio
COM = gcc # Compilador
LIB = -lm -lquadmath # Llibreries (-l*, -L*, -I*)
MAC = -D_GNU_SOURCE # Macros (-D*)
AVS = -W -Wall -Wextra -ansi -pedantic # Avisos
OPT = -march=native -O2 -pipe # Optimitzacio
DEP = -g -DDEBUG # Depuracio, no recomanable junt amb $(OPT)
OPC = $(DEP) $(AVS) $(MAC) -std=c11 # Opcions
DIR = /usr/local/bin # Directori per a instalar

all: $(EXE)

composicio: src/composicio.c metodes.o solar.o molecular.o fput.o harmonic.o config.o
	@echo "\033[0;32m"Compilant i enllaçant $^ per a crear $@"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) $(LIB) $^ -o $@
	@echo -n "\033[0m"

metodes.o: src/metodes.c src/metodes.h src/tipus.h
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

config.o: src/config.c src/config.h src/tipus.h
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

harmonic.o: src/harmonic.c src/harmonic.h src/tipus.h metodes.o
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

fput.o: src/fput.c src/fput.h src/tipus.h metodes.o
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

molecular.o: src/molecular.c src/molecular.h src/tipus.h metodes.o
	@echo "\033[0;36m"Compilant $<"\033[0m"
	@echo -n "  \033[0;37m"
	$(COM) $(OPC) -c $<
	@echo -n "\033[0m"

solar.o: src/solar.c src/solar.h src/tipus.h metodes.o
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
	rm -rf ./dat/*
	@echo -n "\033[0m"

clean_all: clean clean_exe clean_data
