source $1

echo "Distribution:" $dist " Record Count:" $rc " Operation Count:" $oc "Update Proportion:" $up "Insert Proportion" $ip
if [ ! -f testfiles/testfile-$dist-$rc-$oc-$up.txt ]; then
	$YCSB_HOME/bin/ycsb load basic -p recordcount=$rc -P $YCSB_HOME/workloads/$dist > testfiles/testfile-$dist-$rc-$oc-$up.txt
	$YCSB_HOME/bin/ycsb run basic -p recordcount=$rc -p operationcount=$oc -p updateproportion=$up -p insertproportion=$ip -P $YCSB_HOME/workloads/$dist >> testfiles/testfile-$dist-$rc-$oc-$up.txt
fi
./main ycsbfile testfiles/testfile-$dist-$rc-$oc-$up.txt $1 > result/fileresult-$dist-$rc-$oc-$up.txt  
#./main ycsbnumber testfiles/testfile-$dist-$rc-$oc-$up.txt $1 > result/numberresult-$dist-$rc-$oc-$up.txt
#./set-merge/myprog set-merge/testfiles/testfile-$dist-$rc-$oc-$up.txt > set-merge/result/result-$dist-$rc-$oc-$up.txt
rm logs/*
