#!/bin/bash
# 27-06-2018
# cosmogat
# .comu.sh
# Coses comunes als guions

function info {
    echo "Utilitzar:"
    echo "$0 [op]"
    echo "On [op] és:"
    echo -e "\t 0 -> Tots els mètodes."
    echo -e "\t 1 -> Mètodes 1-4"
    echo -e "\t 2 -> Mètodes 5-7"
    echo -e "\t 3 -> Mètodes 8-14"
    echo -e "\t 4 -> Mètodes 15-17"
    echo -e "\t 5 -> Mètodes 18-23"
}

OPC=0
CONF="ext18"
FITX=()
METO=()

if [ $# -gt 1 ] ; then
    info
    exit 1
elif [ $# -eq 1 ] ; then
    OPC=$1
fi

case $OPC in
    0)
	DIR_ANT=$PWD
	cd ../src
	for i in $(ls *.c) ; do
	    if [ "$i" != "solar.c" ] ; then
		NOM_FIT=$(basename $i .c)
		FITX+=($NOM_FIT)
	    fi
	done
	cd $DIR_ANT
	;;
    1)
	FITX[0]="01_expl"
	FITX[1]="02_simp"
	FITX[2]="03_stor"
	FITX[3]="04_llib"	
	;;
    2)
	FITX[0]="05_rk-4"
	FITX[1]="06_rkn4"
	FITX[2]="07_rkg4"
	;;
    3)
	FITX[0]="08_tjc4"
	FITX[1]="09_ss45"
	FITX[2]="10_ss69"
	FITX[3]="11_ss817"
	FITX[4]="12_s46"
	FITX[5]="13_s46s"
	FITX[6]="14_nb46"	
	;;
    4)
	FITX[0]="15_nia2"
	FITX[1]="16_nia5"
	FITX[2]="17_nia8"	
	;;
    5)
	FITX[0]="18_pss613"
	FITX[1]="19_ps44"
	;;
    *)
	info
	exit 2
	;;
esac

for i in ${FITX[@]} ; do
    NOM_MET=$(echo $i | cut -d'_' -f2)
    METO+=($NOM_MET)
done
