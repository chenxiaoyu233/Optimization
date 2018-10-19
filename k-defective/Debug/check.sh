#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective gen "20" "0.1"
	K=`expr $RANDOM % 60`
	echo k: $K
	./KDefective RDS main.in $K > bao.out
	./KDefective algo main.in $K > algo.out

	echo Bao:
	cat bao.out

	echo Algo:
	cat algo.out

	diff bao.out algo.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac $i
	fi
done
