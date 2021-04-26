#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --mem=8G
#SBATCH --nodes=8
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=1
#SBATCH --job-name conduit-internode
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A.out"
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=mmore500@msu.edu

export CONTEXT=icer

zsh <(curl -s https://raw.githubusercontent.com/mmore500/conduit/master/microbenchmarks/slurm/job.sh\?$(date +%s))
