#!/bin/sh
#SBATCH --partition=FastPool
#SBATCH --job-name=CAData
#SBATCH --array=0-99
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --output=output.log

srun ./commands4.sh $SLURM_ARRAY_TASK_ID