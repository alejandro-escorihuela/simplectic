#!/bin/bash
# 15-06-2018
# alex
# plot.sh

CONF="ext18"
DIR_ANT=$PWD
PLANETES=()
PLANETES[0]="sol"
cd ../cnf
for i in $(cat $CONF.cnf | cut -d' ' -f1) ; do
    if [ "$i" != "#Nom" ] ; then
	PLNT=$(echo $i | cut -c1-3)
	PLANETES+=($PLNT)
    fi
done
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

touch _plot.plt
echo "reset" >> _plot.plt
echo "set term pdf" >> _plot.plt
echo "set output '../graf/$CONF.pdf'" >> _plot.plt
echo "unset key" >> _plot.plt
echo "unset tics" >> _plot.plt
echo "unset border" >> _plot.plt
echo "set style line 1  lt rgb '#f3f675'" >> _plot.plt
echo "set style line 2  lt rgb '#c2a677'" >> _plot.plt
echo "set style line 3  lt rgb '#e7d372'" >> _plot.plt
echo "set style line 4  lt rgb '#7ce5eb'" >> _plot.plt
echo "set style line 5  lt rgb '#609db0'" >> _plot.plt
echo "set style line 6  lt rgb '#acafb6'" >> _plot.plt
echo "set style line 7  lt rgb '#bdbfbf'" >> _plot.plt
echo "set style line 8  lt rgb '#dfb26d'" >> _plot.plt
echo "set style line 9  lt rgb '#95b95f'" >> _plot.plt
echo "set style line 10  lt rgb '#c65b4c'" >> _plot.plt
echo "set style line 11 lc rgb '#f3f675' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 12 lc rgb '#c2a677' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 13 lc rgb '#e7d372' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 14 lc rgb '#7ce5eb' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 15 lc rgb '#609db0' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 16 lc rgb '#acafb6' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 17 lc rgb '#bdbfbf' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 18 lc rgb '#dfb26d' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 19 lc rgb '#95b95f' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
echo "set style line 20 lc rgb '#c65b4c' lt 1 lw 2 pt 7 pi -1 ps 1.5" >> _plot.plt
ITi=0
for i in ${FITX[@]} ; do
    echo "set title '${METO[$ITi]}'" >> _plot.plt
    LIN0=$(wc -l "../dat/${i}_$CONF/err.dat" | cut -d' ' -f1)
    let "LIN1=LIN0-1"
    echo "arxiu(i, f, pl) = sprintf(\"<(sed -n '%d,%dp' ../dat/${i}_$CONF/%s.dat)\", i, f, pl)" >> _plot.plt
    ITj=1
    echo -n "splot " >> _plot.plt
    for j in ${PLANETES[@]} ; do
	echo -n "arxiu(1, $LIN0, '$j')    u 2:3:4 w l ls $ITj, " >> _plot.plt
	let "ITj++"
    done
    ITj=11
    for j in ${PLANETES[@]} ; do
	echo -n "arxiu($LIN0, $LIN1, '$j') u 2:3:4 w lp ls $ITj, " >> _plot.plt
	let "ITj++"
    done
    for j in ${PLANETES[@]} ; do
	if [ "$j" != "sol" ] ; then
	    LLETRA=$(echo $j | cut -c1-1)
	    echo -n "arxiu($LIN0, $LIN1, '$j') u 2:3:4:('$LLETRA') with labels textcolor 'white', " >> _plot.plt
	fi
    done
    echo "" >> _plot.plt
    let "ITi++"
done
gnuplot _plot.plt
rm _plot.plt
