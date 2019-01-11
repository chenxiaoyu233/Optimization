#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective -O generate -n 100 -d 0.1 -w main.in
	K=`expr $RANDOM % 5`
	echo k: $K
	./KDefective -O solve -a RDS -D Bitset -k $K -r main.in -p > Bao.out
	./KDefective -O solve -a Base -D Bitset -k $K -r main.in -p > Base.out

	echo RDS:
	cat Bao.out

	echo Base:
	cat Base.out

	#只比较第一行
	head -n 1 Bao.out > Bao.out
	head -n 1 Base.out > Base.out

	diff Bao.out Base.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac $i
	fi
done
