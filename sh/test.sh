#!/bin/bash
# 12-06-2018
# alex
# test.sh

if [ $# -ne 1 ] ; then
    echo "S'ha de passar un valor per a l'increment de temps."
    exit 1
fi

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

PAS=$1
cd ..
make
IT=0
for i in ${FITX[@]} ; do
    echo "${METO[$IT]}"
#    if [ "${METO[$IT]}" != "rkg4" ] ; then
	./$i $PAS
#    fi
    let "IT++"
done
cd -

