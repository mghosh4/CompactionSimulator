#!/bin/sh
source $1
echo $2
rc=$recordcount
fi=$fileperiteration
oc=$operationcount
for dist in "${distribution[@]}"
do
	echo $dist
	for up in "${updateproportion[@]}"
	do
		echo $up | awk '{printf "%d\t", $0}'
		grep "$2" result/comparison*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "SizeTieredStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t%8.7f\t", sum/NR, sqrt((sumsq-sum^2/NR)/NR)}'
		grep "$2" result/comparison*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "GreedyStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t%8.7f\t", sum/NR, sqrt((sumsq-sum^2/NR)/NR)}'
		#grep "$2" result/comparison*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "BalancedTreeSizeTieredStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t%8.7f\t", sum/NR, sqrt((sumsq-sum^2/NR)/NR)}'
		#grep "$2" result/comparison*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "BalancedTreeGreedyStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t%8.7f\t", sum/NR, sqrt((sumsq-sum^2/NR)/NR)}'
		grep "$2" result/comparison*/fileresult-$dist-$rc-$oc-$up.txt | grep -w "RandomStrategy" | awk '{print $3}' | sed s/\:/\ / | awk '{print $2}' | awk '{sum+=$1;sumsq+=($1)^2} END {printf "%8.7f\t%8.7f\n", sum/NR, sqrt((sumsq-sum^2/NR)/NR)}'
	done
done
