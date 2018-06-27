#!/bin/bash
# 26-04-2018
# alex
# ordre.sh

PAS=()
PAS[0]=300
PAS[1]=200
PAS[2]=100
PAS[3]=50
PAS[4]=10

. .comu.sh

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
