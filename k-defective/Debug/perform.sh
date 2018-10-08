#!/bin/bash

for ((i = 1; i; i = i + 1)) ; do
	./KDefective gen "40" "0.6"
	printf "File %d: %s\n" $i "`head -n 1 main.in`"
	for ((k = 1; k <= 20; k = k + 1)) ; do
		printf "k = %d\n" $k
		printf "algo: "
		./KDefective algo main.in $k > bao.out

		printf "RDS: "
		./KDefective RDS main.in $k > algo.out
	done
	echo ""
done
