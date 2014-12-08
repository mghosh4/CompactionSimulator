#!/bin/sh
make clean
make
source $1
rc=$recordcount
fi=$fileperiteration
for dist in "${distribution[@]}"
do
	for up in "${updateproportion[@]}"
	do
		for oc in "${operationcount[@]}"
		do
			for fs in "${filesize[@]}"
			do
				echo "YCSB_HOME=/project/mongo-db-query/YCSB" > conffiles/$1
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
done
