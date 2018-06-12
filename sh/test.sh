#!/bin/bash
# 12-06-2018
# alex
# test.sh

if [ $# -ne 1 ] ; then
    echo "S'ha de passar un valor per a l'increment de temps."
    exit 1
fi
EXEC=()
EXEC[0]="expl"
EXEC[1]="simp"
EXEC[2]="stor"
EXEC[3]="llib"
EXEC[4]="rk-4"
EXEC[5]="rkn4"
EXEC[6]="rkg4"
EXEC[7]="tjc4"
EXEC[8]="ss45"
EXEC[9]="ss69"
EXEC[10]="ss817"
PAS=$1
cd ..
make
for i in ${EXEC[@]} ; do
    echo "$i"
    ./$i $PAS
done
cd -

