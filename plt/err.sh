#!/bin/bash
# 15-06-2018
# alex
# err.sh

. .comu.sh

touch _err.plt
echo "reset" >> _err.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> _err.plt
echo "set key font ',8'" >> _err.plt
echo "set xtics font 'Verdana,8'" >> _err.plt
echo "set output \"../graf/err${OPC}.pdf\"" >> _err.plt
echo "set key out vert" >> _err.plt
echo "set key right" >> _err.plt
echo "set xrange[-0.25:2.75]" >> _err.plt
echo "set title \"Error in the energy ($CONF)\"" >> _err.plt
echo "set ylabel \"log((H-H_0)/H_0)\"" >> _err.plt
echo "set xlabel \"log(t) (year)\"" >> _err.plt
echo -n "plot " >> _err.plt
IT=0
for i in ${FITX[@]} ; do
    METODE=${METO[$IT]}
    echo -n "'../dat/${i}_$CONF/err.dat' u (log10(\$1 / 365.25)):(log10(abs(\$3-\$2) / \$2)) t '$METODE' w l, " >> _err.plt
    let "IT++"
done
gnuplot _err.plt
rm _err.plt
