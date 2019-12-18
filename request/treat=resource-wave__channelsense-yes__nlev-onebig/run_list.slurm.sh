#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=167:00:00         # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=1-40
#SBATCH --mem=8G                # memory required per node - amount of memory (in bytes)
#SBATCH --job-name treat=resource-wave__channelsense-yes__nlev-onebig     # you can give your job a name for easier identification (same as -J)
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1000
TREATMENT="treat=resource-wave__channelsense-yes__nlev-onebig"
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

# to store port number
fname=port.$SLURM_JOBID

# start coordinator
dmtcp_coordinator --daemon --exit-on-last -p 0 --port-file $fname $@          \
  1>/dev/null 2>&1

# get coordinator's host name
h=$(hostname)

# get coordinator's port number
p=$(cat $fname)

export DMTCP_COORD_HOST=$h

export DMTCP_COORD_PORT=$p

# checkpoint every six hours
export CKPT_WAIT_SEC=$(( 6 * 60 * 60 ))

################################################################################
echo "Do Work"
################################################################################
module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

# run job in background
dmtcp_launch -h $DMTCP_COORD_HOST -p $DMTCP_COORD_PORT --rm --ckpt-open-files  \
  ./dishtiny -SEED ${SEED}                                                     \
  >run.log 2>&1                                                                \
  &

# while the program is still running checkpoint it intermittently
# while dmtcp_command -h $DMTCP_COORD_HOST -p $DMTCP_COORD_PORT -s               \
#    1>/dev/null 2>&1
# do
#
#   # wait for checkpoint interval
#   sleep $CKPT_WAIT_SEC
#
#   # move any current checkpoint files out of the way
#   mkdir -p checkpoint_bak
#   mv ckpt_*.dmtcp checkpoint_bak
#
#   # checkpoint the job and, if successful, clear old checkpoint files
#   dmtcp_command -h $DMTCP_COORD_HOST -p $DMTCP_COORD_PORT                      \
#     --ckpt-open-files -bc \
#     && rm -rf checkpoint_bak
#
# done

###############################################################################
echo "Done"
################################################################################
