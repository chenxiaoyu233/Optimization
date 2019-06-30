#!/bin/bash
# work.slurm
#SBATCH --job-name=kdefect
#SBATCH --mail-type=end
#SBATCH --mail-user=yi.zhou@univ-angers.fr
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

$1 -O solve -a $2 -t $3 -D $4 -k $5 -r $6 -G $7 -M $8 -p $9 -l $10 > task$11.out 2> task$11.err
