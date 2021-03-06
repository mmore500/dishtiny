#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --mem=8G
#SBATCH --nodes=8
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1
#SBATCH --job-name dishtiny-internode
#SBATCH --account=devolab
#SBATCH --output="/mnt/scratch/%u/slurmlogs/slurm-%A.out"
#SBATCH --mail-type=FAIL
# No --mail-user, the default value is the submitting user

export CONTEXT=icer

zsh <(curl -s https://raw.githubusercontent.com/mmore500/dishtiny/master/microbenchmarks/slurm/nnode=8+nproc=1+ext=.slurm.sh\?$(date +%s))
