#!/bin/bash
# task.slurm
#SBATCH --job-name=kdefect
#SBATCH --mail-type=end
#SBATCH --mail-user=yi.zhou@univ-angers.fr
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/yi.zhou/Gurobi_Env/gurobi811/linux64/lib
$1 -O solve -a $2 -t $3 -D $4 -k $5 -r $6 -G $7 -M $8 -p -l $9 > task${10}.out 2> task${10}.err
