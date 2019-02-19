#!/bin/bash
# 28-01-2019
# alex
# ordre_fput04.sh

POTEN="fput"
TITOL="Eficiència FPUT {/Symbol a}=0.25"
T_MAX=500
PAS=()
MET=()
LLE=()
PAS[0]=0.5
PAS[1]=0.3
PAS[2]=0.2
PAS[3]=0.1
PAS[4]=0.06
PAS[5]=0.03
PAS[6]=0.01
MET[0]="nb_6_4"
MET[1]="sa_8_4"
MET[2]="psa_3_6_4"
MET[3]="nia_8"
MET[4]="pnia_3_6"
LLE[0]="_6^{[4]}"
LLE[1]="_8^{[4]}"
LLE[2]="_{3,6}^{[4]}"
LLE[3]="_{8}^{[10,6,4]}"
LLE[4]="_{3,6}^{[7,6,4]}"

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
echo "set title \"$TITOL\"" >> plot.plt
echo "set ylabel \"log(màx(H-H_0)/H_0)\"" >> plot.plt
echo "set xlabel \"log(N_{eval})\"" >> plot.plt
echo -n "plot " >> plot.plt
IT=0
for i in ${MET[@]} ; do
    k=$(echo $i | cut -d'_' -f 1 | tr [[:lower:]] [[:upper:]])
    l=${LLE[$IT]}
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$k$l\" w lp lw 2 ps 0.5 pt 2, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
