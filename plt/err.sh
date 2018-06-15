#!/bin/bash
# 15-06-2018
# alex
# err.sh

METO=()
METO[0]="expl"
METO[1]="simp"
METO[2]="stor"
METO[3]="llib"
METO[4]="rk-4"
METO[5]="rkn4"
METO[6]="rkg4"
METO[7]="tjc4"
METO[8]="ss45"
METO[9]="ss69"
METO[10]="ss817"
CONF="ext18"

touch _err.plt
echo "reset" >> _err.plt
echo "set term pdf" >> _err.plt
echo "set output \"../graf/err_ext18.pdf\"" >> _err.plt
echo "set key out vert" >> _err.plt
echo "set key right" >> _err.plt
echo "set xrange[-0.25:2.75]" >> _err.plt
echo "set title \"Error en l'energia ($CONF)\"" >> _err.plt
echo "set ylabel \"log((H-H_0)/H_0)\"" >> _err.plt
echo "set xlabel \"log(t) (anys)\"" >> _err.plt
echo -n "plot " >> _err.plt
for i in ${METO[@]} ; do
    echo -n "'../dat/${i}_$CONF/err.dat' u (log10(\$1 / 365.25)):(log10(abs(\$3-\$2) / \$2)) t '$i' w l, " >> _err.plt
done
gnuplot _err.plt
rm _err.plt
