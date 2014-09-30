#!/bin/sh
source setup.conf
echo ${distribution[@]}
for j in "${distribution[@]}"
do
	for k in "${recordcount[@]}"
	do
		echo $j $k
		$YCSB_HOME/bin/ycsb load basic -p recordcount=$k -P $YCSB_HOME/workloads/$j > testfile_$j_$k.txt
		$YCSB_HOME/bin/ycsb run basic -p recordcount=$k -P $YCSB_HOME/workloads/$j >> testfile_$j_$k.txt
		./main YCSB testfile_$j_$k.txt
	done
done
