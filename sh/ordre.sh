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
#PAS[5]=0.5
#PAS[6]=0.1
EXE=()
EXE[0]="expl"
EXE[1]="simp"
EXE[2]="stor"
EXE[3]="llib"
EXE[4]="rk-4"
EXE[5]="rkn4"
EXE[6]="rkg4"
EXE[7]="tjc4"
EXE[8]="ss45"
EXE[9]="ss69"
EXE[10]="ss817"

cd ..
mkdir .tmp_dat
for i in ${EXE[@]} ; do
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
# echo "set style line 1  lt rgb '#0060AD'" >> plot.plt
# echo "set style line 2  lt rgb '#60AD00'" >> plot.plt
# echo "set style line 3  lt rgb '#AD0B00'" >> plot.plt
# echo "set style line 4  lt rgb '#E7E730'" >> plot.plt
# echo "set style line 5  lt rgb '#c2a677'" >> plot.plt
# echo "set style line 6  lt rgb '#e7d372'" >> plot.plt
# echo "set style line 7  lt rgb '#7ce5eb'" >> plot.plt
# echo "set style line 8  lt rgb '#609db0'" >> plot.plt
# echo "set style line 9  lt rgb '#acafb6'" >> plot.plt
echo -n "plot " >> plot.plt
IT=0
for i in ${EXE[@]} ; do
    let "IT++"
    #echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$i\" w lp ls $IT, " >> plot.plt
    echo -n "\"$i.dat\" u (log10(\$3)):(log10(\$4)) t \"$i\" w lp, " >> plot.plt
done
gnuplot plot.plt
rm plot.plt
cd ..
rm -rf .tmp_dat
