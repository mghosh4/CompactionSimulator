#!/bin/sh
source $1
rc=$recordcount
fc=$filecount
up=$updateproportion
for dist in "${distribution[@]}"
do
	echo $dist
	for fs in "${filesize[@]}"
	do
		oc=$(($fc * $fs))
		#awk -v var=$up '{printf "%d\t", var}'
		grep "$2" result/optimal*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "BalancedTreeSizeTieredStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t", sum/NR}'
		grep "$2" result/optimal*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "Lower" | awk '{print $8}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\n", sum/NR}'
	done
done
