#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective -O generate -n 10 -d 0.3 -w main.in
	K=`expr $RANDOM % 5 + 1`
	echo k: $K
	./KDefective -O solve -a RDS -D Bitset -k $K -r main.in -p > RDS.out
	./KDefective -O solve -a Base -D Bitset -k $K -r main.in -p > Base.out
	./KDefective -O solve -a IP -D Bitset -k $K -r main.in -l IP.out

	echo RDS:
	cat RDS.out

	echo Base:
	cat Base.out

	echo IP:
	cat IP.out

	#只比较第一行
	head -n 1 RDS.out > RDS.out
	head -n 1 Base.out > Base.out
	head -n 1 IP.out > IP.out

	diff RDS.out Base.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac [RDS Base] $i
	fi

	diff RDS.out IP.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac [RDS IP] $i
	fi
done
