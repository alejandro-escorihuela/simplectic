#!/bin/bash
# 12-03-2019
# alex
# ordre_solar_comp.sh

POTEN="solar"
T_MAX=200000
PAS=()
MET=()
PAS[0]=500
PAS[1]=200
PAS[2]=100
PAS[3]=50
PAS[4]=25
PAS[5]=10
MET[0]="sx_6_4"
MET[1]="sx_6_4_e"
MET[2]="sx_6_4_k"
MET[3]="sx_5_4"
MET[4]="sx_5_4_e"
MET[5]="sx_5_4_k"
TITOLS[0]="S_6^{[4]}"
TITOLS[1]="X_{(6,E)}^{[4]}"
TITOLS[2]="X_{(6,k)}^{[4]}"
TITOLS[3]="SS_5^{[4]}"
TITOLS[4]="X_{(5,E)}^{[4]}"
TITOLS[5]="X_{(5,k)}^{[4]}"

cd ..
mkdir .tmp_dat
for i in ${MET[@]} ; do
    for j in ${PAS[@]} ; do
	k=$(echo $i | cut -d'_' -f 1)
	l=$(echo $i | cut -d'_' -f 2-)
	echo "Execució de simplectic (tipus = $k, coeficients = $l, h = $j)"
	./simplectic $POTEN $k $l $j $T_MAX 1 >> .tmp_dat/$i.dat
    done
done
cd .tmp_dat

echo "reset" >> plot.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> plot.plt
echo "set key font ',8'" >> plot.plt
echo "set xtics font 'Verdana,8'" >> plot.plt
echo "set output \"../graf/ordre_solar_comp.pdf\"" >> plot.plt
echo "set key out vert" >> plot.plt
echo "set key right" >> plot.plt
echo "set title \"Eficiència en Sistema Solar exterior\"" >> plot.plt
echo "set ylabel \"log(max(H-H_0)/H_0)\"" >> plot.plt
echo "set xlabel \"log(N_{eval})\"" >> plot.plt
echo -n "plot " >> plot.plt
IT=0
for i in ${MET[@]} ; do
    TIT=${TITOLS[$IT]}
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$TIT\" w lp lw 2 ps 0.5 pt 2, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
