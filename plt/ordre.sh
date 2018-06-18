#!/bin/bash
# 26-04-2018
# alex
# ordre.sh

PAS=()
PAS[0]=100
PAS[1]=50
PAS[2]=10
PAS[3]=5
PAS[4]=1
# PAS[5]=0.5
# PAS[6]=0.1
FITX=()
METO=()
DIR_ANT=$PWD
cd ../src
for i in $(ls *.c) ; do
    if [ "$i" != "solar.c" ] ; then
	NOM_FIT=$(basename $i .c)
	NOM_MET=$(echo $NOM_FIT | cut -d'_' -f2)
	FITX+=($NOM_FIT)
	METO+=($NOM_MET)
    fi
done
cd $DIR_ANT

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
echo "set term pdf" >> plot.plt
echo "set output \"../graf/ordre.pdf\"" >> plot.plt
echo "set key out vert" >> plot.plt
echo "set key right" >> plot.plt
echo "set title \"Eficiència\"" >> plot.plt
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
