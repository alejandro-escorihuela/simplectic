#!/bin/bash
# 26-04-2018
# alex
# ordre_solar.sh

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
MET[0]="na_7_6_1"
MET[1]="na_7_6_2"
MET[2]="na_7_6_3"
MET[3]="na_7_6_4"
MET[4]="na_7_6_5"
MET[5]="na_7_6_6"
MET[6]="na_7_6_7"
MET[7]="na_7_6_8"
MET[8]="na_7_6_9"
MET[9]="na_7_6_10"
MET[10]="na_7_6_11"
MET[11]="na_7_6_12"
MET[12]="na_7_6_13"
MET[13]="na_7_6_14"
MET[14]="na_7_6_15"
MET[15]="na_7_6_16"
MET[16]="na_7_6_17"
MET[17]="na_7_6_18"
MET[18]="na_7_6_19"
MET[19]="na_7_6_20"
MET[20]="na_7_6_21"
MET[21]="na_7_6_22"
MET[22]="na_7_6_23"

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
echo "set output \"../graf/ordre_solar.pdf\"" >> plot.plt
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
    #n=$(echo $i | cut -d'_' -f 4)
    l="_$l^{[$m]}"
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$k$l $n\" w lp lw 2 ps 0.5 pt 2, " >> plot.plt
    let "IT++"
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
