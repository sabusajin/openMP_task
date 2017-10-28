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


N="1000000000"
THREADS="1 16"

make reduce

for n in $N;
do
    for t in $THREADS;
    do
	./bubblesort $n $t >/dev/null 2> ${RESULTDIR}/bubblesort_${n}_${t}
    done
done
value=`cat ${RESULTDIR}/bubblesort_1000000000_1`
echo "$value"
value2=`cat ${RESULTDIR}/bubblesort_1000000000_16`
echo "$value2"
RESULT=$(awk "BEGIN {printf \"%.2f\",${value}/${value2}}")
echo $RESULT
