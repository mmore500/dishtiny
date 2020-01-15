#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-99
#SBATCH --mem=16G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 4
#SBATCH --job-name s-start
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

################################################################################
echo "Prepare Env Vars"
################################################################################

SEED_OFFSET=1000
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))
CUR_STEP=0

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED}+step=0"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/screen/"

echo "   TREATMENT" $TREATMENT
echo "   SEED" $SEED
echo "   CUR_STEP" $CUR_STEP
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

################################################################################
echo "Submit Next Job..."
################################################################################

# export information so jinja can use it
export seed=$SEED
export last_step=$CUR_STEP # aka cur step wrt this script
export cur_step=$(( $CUR_STEP + 1 )) # aka next step wrt this script

echo "   seed" $seed
echo "   last_step" $last_step
echo "   cur_step" $cur_step

# prepare python environment
module purge; module load GCC/7.3.0-2.30 OpenMPI/3.1.1 Python/3.6.6
source "/mnt/home/mmore500/myPy/bin/activate"

# run jinja on template
j2 -o run_list.slurm.sh run_list.slurm.sh.jinja

if  [ $CUR_STEP -lt 24 ]; then
  if ls *.json.cereal | grep -q "update\=16384"; then
    sbatch run_list.slurm.sh                                                   \
    && echo "   job submit success!"                                           \
    || echo "   job submit failure (FAIL)"
  else
    echo "   missing final population... job incomplete? (FAIL)"
  fi
else
  echo "   nevermind, all done! (COMPLETE)"
fi

###############################################################################
echo "Done" $(date)
################################################################################

qstat -f ${PBS_JOBID}

cp "/mnt/home/mmore500/slurmlogs/slurm-${PBS_JOBID}_${PBS_ARRAYID}.out" .
