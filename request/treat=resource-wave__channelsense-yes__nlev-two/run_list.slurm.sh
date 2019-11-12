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
echo "Setup DMTCP"
# https://wiki.hpcc.msu.edu/display/ITH/Checkpoint+with+DMTCP
################################################################################

# set a limited stack size so DMTCP could work
ulimit -s 8192

# save coordinators host info in an environment variable
export DMTCP_COORD_HOST=$(hostname)

# ask for a uniqe port at runtime
export DMTCP_COORD_PORT=0

# checkpoint every eight hours
CKPT_WAIT_SEC=$(( 8 * 60 * 60 ))

# back up checkpoints every hour
BAK_WAIT_SEC=$(( 1 * 60 * 60 ))

echo "   DMTCP_COORD_HOST" $DMTCP_COORD_HOST
echo "   DMTCP_COORD_PORT" $DMTCP_COORD_PORT
echo "   CKPT_WAIT_SEC" $CKPT_WAIT_SEC
echo "   BAK_WAIT_SEC" $BAK_WAIT_SEC

################################################################################
echo "Do Work"
################################################################################
module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

# set up background loop
# so we always have at least one previous checkpoint and file state
mkdir checkpoint_bak
touch checkpoint_bak/init.dmtcp
while true; do
  echo "   BAK SLEEP" $(date)
  sleep $BAK_WAIT_SEC
  echo "   BAK WAKE" $(date)
  # if we have a new checkpoint, shuffle the backups
  if ! diff *.dmtcp checkpoint_bak/*.dmtcp; then
    echo "   BAK BEGIN" $(date)
    rm -rf checkpoint_bakbak
    mv checkpoint_bak checkpoint_bakbak
    mkdir checkpoint_bak
    cp *.csv *.h5 *dmtcp* checkpoint_bak
    echo "   BAK END" $(date)
  fi
done &

# run job with checkpointing
dmtcp_launch -i $CKPT_WAIT_SEC                                                 \
  ./dishtiny -SEED ${SEED} -ANIMATION_FREQUENCY 2097152                        \
  >run.log 2>&1

# when running job fails or finishes, kill the background loop
pkill -P $$

###############################################################################
echo "Done" $(date)
################################################################################
