#!/bin/bash
# 15-06-2018
# alex
# err.sh

CONF="ext18"
DIR_ANT=$PWD
FITX=()
METO=()
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
