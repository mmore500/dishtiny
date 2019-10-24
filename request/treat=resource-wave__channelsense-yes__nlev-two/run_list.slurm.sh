#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=167:00:00         # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=1-40
#SBATCH --mem=8G                # memory required per node - amount of memory (in bytes)
#SBATCH --job-name treat=resource-wave__channelsense-yes__nlev-two     # you can give your job a name for easier identification (same as -J)
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1000
TREATMENT="treat=resource-wave__channelsense-yes__nlev-two"
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny/"${TREATMENT}"/"${SEED}"/"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/request/"${TREATMENT}

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
./dishtiny -SEED ${SEED} >run.log 2>&1

###############################################################################
echo "Done"
################################################################################
