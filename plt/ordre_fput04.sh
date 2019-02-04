#!/bin/bash
# 28-01-2019
# alex
# ordre_fput04.sh

POTEN="fput"
T_MAX=500
PAS=()
MET=()
PAS[0]=0.5
PAS[1]=0.3
PAS[2]=0.2
PAS[3]=0.1
PAS[4]=0.06
PAS[5]=0.03
PAS[6]=0.01
MET[0]="psa_3_3_4"
MET[1]="psa_4_4_4"


cd ..
mkdir .tmp_dat
for i in ${MET[@]} ; do
    for j in ${PAS[@]} ; do
	k=$(echo $i | cut -d'_' -f 1)
	l=$(echo $i | cut -d'_' -f 2-)
	m=$(bc -l <<< "($T_MAX/$j)*0.01")
	m=$(echo $m | cut -d'.' -f1)
	echo "Execució de simplectic (tipus = $k, coeficients = $l, h = $j, iteracions = $m)"
	./simplectic $POTEN $k $l $j $T_MAX $m >> .tmp_dat/$i.dat
    done
done
cd .tmp_dat

echo "reset" >> plot.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> plot.plt
echo "set key font ',8'" >> plot.plt
echo "set xtics font 'Verdana,8'" >> plot.plt
echo "set output \"../graf/ordre_fput04.pdf\"" >> plot.plt
echo "set key out vert" >> plot.plt
echo "set key right" >> plot.plt
echo "set title \"Efficiency\"" >> plot.plt
echo "set ylabel \"log(max(H-H_0)/H_0)\"" >> plot.plt
echo "set xlabel \"log(N_{eval})\"" >> plot.plt
echo -n "plot " >> plot.plt
IT=0
for i in ${MET[@]} ; do
    k=$(echo $i | cut -d'_' -f 1 | tr [[:lower:]] [[:upper:]])
    l=$(echo $i | cut -d'_' -f 2)
    m=$(echo $i | cut -d'_' -f 3)
    n=$(echo $i | cut -d'_' -f 4)
    l="_{$l,$m}^{[$n]}"
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$k$l\" w lp lw 2 ps 0.5 pt 2, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
