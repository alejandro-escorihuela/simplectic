#!/bin/bash
# 26-04-2018
# alex
# ordre.sh
function info {
    echo "Utilitzar:"
    echo "$0 [op]"
    echo "On [op] és:"
    echo -e "\t 0 -> Tots els mètodes."
    echo -e "\t 1 -> Mètodes 1-4"
    echo -e "\t 2 -> Mètodes 5-7"
    echo -e "\t 3 -> Mètodes 8-17"
}
OPC=0
PAS=()
PAS[0]=300
PAS[1]=200
PAS[2]=100
PAS[3]=50
PAS[4]=10
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
	PAS[5]=0.5	
	;;
    2)
	FITX[0]="05_rk-4"
	FITX[1]="06_rkn4"
	FITX[2]="07_rkg4"
	PAS[5]=0.5
	;;
    3)
	FITX[0]="08_tjc4"
	FITX[1]="09_ss45"
	FITX[2]="10_ss69"
	FITX[3]="11_ss817"
	FITX[4]="12_s46"
	FITX[5]="13_s46s"
	FITX[6]="14_nb46"
	FITX[7]="15_nia2"
	FITX[8]="16_nia5"
	FITX[9]="17_nia8"	
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


cd ..
mkdir .tmp_dat
for i in ${FITX[@]} ; do
    for j in ${PAS[@]} ; do
	echo "Execució de $i amb h=$j"
	./$i $j >> .tmp_dat/$i.dat
    done
done
cd .tmp_dat
echo "reset" >> plot.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> plot.plt
echo "set key font ',8'" >> plot.plt
echo "set xtics font 'Verdana,8'" >> plot.plt
echo "set output \"../graf/ordre${OPC}.pdf\"" >> plot.plt
echo "set key out vert" >> plot.plt
echo "set key right" >> plot.plt
echo "set title \"Efficiency\"" >> plot.plt
echo "set ylabel \"log(max(H-H_0)/H_0)\"" >> plot.plt
echo "set xlabel \"log(N_{eval})\"" >> plot.plt
echo -n "plot " >> plot.plt
IT=0
for i in ${FITX[@]} ; do
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"${METO[$IT]}\" w lp, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
