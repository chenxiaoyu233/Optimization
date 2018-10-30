#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective -O generate -n 20 -d 0.1 -w main.in
	K=`expr $RANDOM % 60`
	echo k: $K
	./KDefective -O solve -a Bao -D Bitset -k $K -r main.in > bao.out
	./KDefective -O solve -a Base -D Bitset -k $K -r main.in > algo.out

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
