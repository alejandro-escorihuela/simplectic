#!/bin/bash
# 20-11-2018
# alex
# ordre5s_molecular.sh

POTEN="molecular"
T_MAX=0.02
PAS=()
MET=()
PAS[0]=5.0e-5
PAS[1]=2.5e-5
PAS[2]=1.0e-5
PAS[3]=7.5e-6
PAS[4]=5.0e-6
PAS[5]=2.5e-6
PAS[6]=1.0e-6
PAS[7]=7.5e-7
MET[0]="sa_5_4_0"
MET[1]="sa_5_4_1"
MET[2]="sa_5_4_2"
MET[3]="sa_5_4_3"
MET[4]="sa_5_4_4"

cd ..
mkdir .tmp_dat
for i in ${MET[@]} ; do
    for j in ${PAS[@]} ; do
	k=$(echo $i | cut -d'_' -f 1)
	l=$(echo $i | cut -d'_' -f 2-)
	echo "Execució de composicio (tipus = $k, coeficients = $l, h = $j)"
	./composicio $POTEN $k $l $j $T_MAX >> .tmp_dat/$i.dat
    done
done
cd .tmp_dat

echo "reset" >> plot.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> plot.plt
echo "set key font ',8'" >> plot.plt
echo "set xtics font 'Verdana,8'" >> plot.plt
echo "set output \"../graf/ordre5s_molecular.pdf\"" >> plot.plt
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
    l="_$l^{[$m]}"
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$k$l $n\" w lp lw 2 ps 0.5 pt 2, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
