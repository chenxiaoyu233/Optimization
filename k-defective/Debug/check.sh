#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective gen "20" "0.6"
	K=`expr $RANDOM % 7`
	echo k: $K
	./KDefective algo main.in $K > bao.out
	./KDefective Simple main.in $K > algo.out

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
