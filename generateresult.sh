#!/bin/sh
source setup.conf
for dist in "${distribution[@]}"
do
	for rc in "${recordcount[@]}"
	do
		for oc in "${operationcount[@]}"
		do
			echo "Distribution:" $dist " Record Count:" $rc " Operation Count:" $oc
			if [ ! -f testfiles/testfile-$dist-$rc-$oc.txt ]; then
				$YCSB_HOME/bin/ycsb load basic -p recordcount=$rc -P $YCSB_HOME/workloads/$dist > testfiles/testfile-$dist-$rc-$oc.txt
				$YCSB_HOME/bin/ycsb run basic -p operationcount=$oc -P $YCSB_HOME/workloads/$dist >> testfiles/testfile-$dist-$rc-$oc.txt
			fi
			#./main ycsbfile testfiles/testfile-$j-$k.txt
			./main ycsbnumber testfiles/testfile-$dist-$rc-$oc.txt > result/result-$dist-$rc-$oc.txt
			#rm logs/*
		done
	done
done
