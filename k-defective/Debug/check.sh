#!/bin/bash

for ((i = 1; i; i = i + 1))
do
	./KDefective -O generate -n 100 -d 0.1 -w main.in
	K=`expr $RANDOM % 5 + 1`
	echo k: $K
	./KDefective -O solve -a RDS -D Bitset -k $K -r main.in -p -l RDS.out 2> /dev/null > /dev/null
	./KDefective -O solve -a Base -D Bitset -k $K -r main.in -p -l Base.out 2> /dev/null > /dev/null
	./KDefective -O solve -a IP -D Bitset -k $K -r main.in -p -l IP.out 2> /dev/null > /dev/null
	#./KDefective -O solve -a Gurobi -D Bitset -k $K -r main.in -p -l Gurobi.out 2> /dev/null > /dev/null
	./KDefective -O solve -a CPLEX -D Bitset -k $K -r main.in -p -l CPLEX.out 2> /dev/null > /dev/null
	./KDefective -O solve -a R2MC -D Bitset -k $K -r main.in -p -l R2MC.out 2> /dev/null > /dev/null

	echo RDS:
	cat RDS.out

	echo Base:
	cat Base.out

	echo IP:
	cat IP.out

    #echo Gurobi:
    #cat Gurobi.out

    echo CPLEX
    cat CPLEX.out

    echo R2MC
    cat R2MC.out

	#只比较第一行
	head -n 1 RDS.out > RDS.out
	head -n 1 Base.out > Base.out
	head -n 1 IP.out > IP.out
    #head -n 1 Gurobi.out > Gurobi.out
	head -n 1 CPLEX.out > CPLEX.out
	head -n 1 R2MC.out > R2MC.out

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

    #diff RDS.out Gurobi.out > /dev/null
	#if [ $? -ne 0 ] ; then
	#	echo Wa
	#	exit
	#else
	#	echo Ac [RDS Gurobi] $i
	#fi

	diff RDS.out CPLEX.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac [RDS CPLEX] $i
	fi

	diff RDS.out R2MC.out > /dev/null
	if [ $? -ne 0 ] ; then
		echo Wa
		exit
	else
		echo Ac [RDS R2MC] $i
	fi

done
