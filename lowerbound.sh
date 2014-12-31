#!/bin/sh
#make clean
#make
source $1
rc=$recordcount
fi=$fileperiteration
fc=$filecount
for dist in "${distribution[@]}"
do
	for up in "${updateproportion[@]}"
	do
		for fs in "${filesize[@]}"
		do
			oc=$(($fc * $fs))
			echo "YCSB_HOME=/project/mongo-db-query/YCSB" > conffiles/$1
			echo "OUTPUT_FLDR=/project/mongo-db-query/CompactionSimulator/result/costfn2" >> conffiles/$1
			echo "EXECUTABLE=partmain" >> conffiles/$1
			echo "of=logs/lbsstable" >> conffiles/$1
			echo "dist=$dist" >> conffiles/$1
			echo "oc=$oc" >> conffiles/$1
			echo "fs=$fs" >> conffiles/$1
			echo "rc=$rc" >> conffiles/$1
			echo "up=$up" >> conffiles/$1
			echo "fi=$fi" >> conffiles/$1
			ip=$((100 - $up))
			echo "ip=$ip" >> conffiles/$1
			sh runresult.sh conffiles/$1
		done
	done
done
