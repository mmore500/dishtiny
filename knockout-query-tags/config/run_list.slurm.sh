#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00         # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=1-20
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 8
#SBATCH --mem=24G                # memory required per node - amount of memory (in bytes)
#SBATCH --job-name knockout     # you can give your job a name for easier identification (same as -J)
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1010
TREATMENT="treat=standard"
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))
SOURCE_SEED=$(( (SLURM_ARRAY_TASK_ID - 1) / 20 + SEED_OFFSET + 1))

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-knockout-query-tags/${TREATMENT}+source=${SOURCE_SEED}+seed=${SEED}/"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/knockout-query-tags/config"
SOURCE_DIR="/mnt/home/mmore500/dishtiny/knockout-query-tags/treat=standard+seed=${SOURCE_SEED}"

echo "   TREATMENT" $TREATMENT
echo "   SEED" $SEED
echo "   SOURCE_SEED" $SOURCE_SEED
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR
echo "   SOURCE_DIR" $SOURCE_DIR

################################################################################
echo "Setup Work Dir"
################################################################################
rm -rf ${OUTPUT_DIR}/*  #If this is a redo, clear the dir
mkdir -p ${OUTPUT_DIR}
cp -r ${CONFIG_DIR}/* ${OUTPUT_DIR}
cd ${OUTPUT_DIR}

mkdir seedpop
cp ${SOURCE_DIR}/* seedpop

################################################################################
echo "Do Work"
################################################################################
module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

cmp --silent seedpop/* \
  && echo "Equivalent Competitors, Skip Competition" \
  || ./dishtiny -SEED ${SEED} >run.log 2>&1

###############################################################################
echo "Done" $(date)
################################################################################

qstat -f ${PBS_JOBID}
