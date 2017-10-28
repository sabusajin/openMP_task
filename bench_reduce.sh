#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


N="100000000"
THREADS="1 16"

make reduce

for n in $N;
do
    for t in $THREADS;
    do
	./reduce $n $t 1000000 >/dev/null 2> ${RESULTDIR}/reduction_${n}_${t}
    done
done
value=`cat ${RESULTDIR}/reduction_100000000_1`
echo "$value"
value2=`cat ${RESULTDIR}/reduction_100000000_16`
echo "$value2"
RESULT=$(awk "BEGIN {printf \"%.2f\",${value}/${value2}}")
echo $RESULT
