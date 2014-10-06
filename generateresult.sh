#!/bin/sh
source setup.conf
for j in "${distribution[@]}"
do
	for k in "${recordcount[@]}"
	do
		echo $j $k
		$YCSB_HOME/bin/ycsb load basic -p recordcount=$k -P $YCSB_HOME/workloads/$j > testfiles/testfile-$j-$k.txt
		$YCSB_HOME/bin/ycsb run basic -p recordcount=$k -P $YCSB_HOME/workloads/$j >> testfiles/testfile-$j-$k.txt
		./main YCSB testfiles/testfile-$j-$k.txt
	done
done
