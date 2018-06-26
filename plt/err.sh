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


# FITX[0]="15_nia2"
# FITX[1]="16_nia5"
# FITX[2]="17_nia8"
# METO[0]="nia2"
# METO[1]="nia5"
# METO[2]="nia8"

touch _err.plt
echo "reset" >> _err.plt
echo "set terminal pdf enhanced font 'Verdana, 10'" >> _err.plt
echo "set key font ',8'" >> _err.plt
echo "set xtics font 'Verdana,8'" >> _err.plt
echo "set output \"../graf/err_ext18.pdf\"" >> _err.plt
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
