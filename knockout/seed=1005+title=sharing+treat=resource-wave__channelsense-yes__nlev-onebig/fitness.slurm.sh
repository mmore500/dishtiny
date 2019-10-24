#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=48:00:00
#SBATCH --array=1-20
#SBATCH --mem=8G
#SBATCH --job-name ko-1005-sharing
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1000
TREATMENT="seed=1005+title=sharing+treat=resource-wave__channelsense-yes__nlev-onebig"
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-knockout/"${TREATMENT}"/"${SEED}"/"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/knockout/"${TREATMENT}"/"

echo "   TREATMENT" $TREATMENT
echo "   SEED" $SEED
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR

################################################################################
echo "Setup Work Dir"
################################################################################
rm -rf ${OUTPUT_DIR}/*  #If this is a redo, clear the dir
mkdir -p ${OUTPUT_DIR}
cp -r ${CONFIG_DIR}/* ${OUTPUT_DIR}
cd ${OUTPUT_DIR}

################################################################################
echo "Do Work"
################################################################################
module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;
./dishtiny -SEED ${SEED} -RUN_LENGTH 270336 >run.log 2>&1

###############################################################################
echo "Done"
################################################################################
