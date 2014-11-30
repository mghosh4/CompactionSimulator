#!/bin/sh
source setup.conf
rc=$recordcount
oc=$operationcount
up=$updateproportion
for dist in "${distribution[@]}"
do
#	for up in "${updateproportion[@]}"
#	do
	#for rc in "${recordcount[@]}"
	#do
	#	for oc in "${operationcount[@]}"
	#	do
			ip=$((100 - $up))
			echo "Distribution:" $dist " Record Count:" $rc " Operation Count:" $oc "Update Proportion:" $up "Insert Proportion" $ip
			if [ ! -f testfiles/testfile-$dist-$rc-$oc-$up.txt ]; then
				$YCSB_HOME/bin/ycsb load basic -p recordcount=$rc -P $YCSB_HOME/workloads/$dist > testfiles/testfile-$dist-$rc-$oc-$up.txt
				$YCSB_HOME/bin/ycsb run basic -p recordcount=$rc -p operationcount=$oc -p updateproportion=$up -p insertproportion=$ip -P $YCSB_HOME/workloads/$dist >> testfiles/testfile-$dist-$rc-$oc-$up.txt
			fi
			./main ycsbfile testfiles/testfile-$dist-$rc-$oc-$up.txt $up > result/greedyfile-$dist-$rc-$oc-$up.txt  
			./main ycsbnumber testfiles/testfile-$dist-$rc-$oc-$up.txt $up > result/greedynumber-$dist-$rc-$oc-$up.txt
			#./set-merge/myprog set-merge/testfiles/testfile-$dist-$rc-$oc-$up.txt > set-merge/result/result-$dist-$rc-$oc-$up.txt
			#rm logs/*
#		done
#	done
done
