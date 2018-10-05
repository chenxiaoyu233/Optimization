#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective gen "30" "0.6"
	K=`expr $RANDOM % 7`
	echo k: $K
	./KDefective bao main.in $K > bao.out
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
