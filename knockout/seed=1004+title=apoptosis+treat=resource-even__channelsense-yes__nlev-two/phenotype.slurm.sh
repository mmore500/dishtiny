#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=1-2
#SBATCH --mem=8G
#SBATCH --job-name ko-1004-apoptosis-phenotype
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

TREATMENT="seed=1004+title=apoptosis+treat=resource-even__channelsense-yes__nlev-two"
SEED=1

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-knockout/"${TREATMENT}"/id="${SLURM_ARRAY_TASK_ID}"/"
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
./dishtiny -SEED ${SEED} -RUN_LENGTH 8192 >run.log 2>&1
