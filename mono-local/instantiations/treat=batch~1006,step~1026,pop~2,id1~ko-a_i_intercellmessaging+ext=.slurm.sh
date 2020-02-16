#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=2:00:00
#SBATCH --mem=6G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 1
#SBATCH --job-name batch~1006,step~1026,pop~2,id1~ko-a_i_intercellmessaging
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A.out"
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=mmore500@msu.edu

################################################################################
echo
echo "Setup Exit and Error Traps"
echo "--------------------------"
################################################################################

function on_exit() {

  echo
  echo "Run Exit Trap"
  echo "-------------"

  qstat -f ${SLURM_JOB_ID}

  # prepare python environment
  module purge; module load GCC/7.3.0-2.30 OpenMPI/3.1.1 Python/3.6.6
  source "/mnt/home/mmore500/myPy/bin/activate"

  echo "   SECONDS" $SECONDS
  echo "   MINUTES" $(python3 -c "print( ${SECONDS}/60 )")
  echo "   HOURS  " $(python3 -c "print( ${SECONDS}/3600 )")

  cp ${SLURM_LOGPATH} .

}

function on_error() {

  echo
  echo "Run Error Trap (FAIL)"
  echo "---------------------"

  echo "   EXIT STATUS ${1}"
  echo "   LINE NO ${2}"

  cp ${SLURM_LOGPATH} "/mnt/home/mmore500/err_slurmlogs"

  qstat -f ${SLURM_JOB_ID}                                                     \
    >> "/mnt/home/mmore500/err_slurmlogs/${SLURM_LOGFILE}"

  echo "---------------------"
  echo

}

trap 'on_error $? $LINENO' ERR
trap "on_exit" EXIT

################################################################################
echo
echo "Prepare Env Vars"
echo "----------------"
################################################################################

SEED_OFFSET=1000
SEED=$((SEED_OFFSET))

OUTPUT_DIR="/mnt/scratch/mmore500/mono-local/treat=batch~1006,step~1026,pop~2,id1~ko-a_i_intercellmessaging+seed=${SEED}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/mono-local"

echo "   SEED" $SEED
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR

export SLURM_LOGFILE="slurm-${SLURM_JOB_ID}.out"
export SLURM_LOGPATH="/mnt/home/mmore500/slurmlogs/${SLURM_LOGFILE}"

echo "   SLURM_LOGFILE" $SLURM_LOGFILE
echo "   SLURM_LOGPATH" $SLURM_LOGPATH

################################################################################
echo
echo "Setup Work Dir"
echo "--------------"
################################################################################

rm -rf ${OUTPUT_DIR}/* || echo "   not a redo"
mkdir -p ${OUTPUT_DIR}
cd ${OUTPUT_DIR}
tar -xvf "${CONFIG_DIR}/treat=batch~1006,step~1026,pop~2,id1~ko-a_i_intercellmessaging+ext=.tar.gz"
mv treatment_directory/* .
rm -rf treatment_directory
cp ${CONFIG_DIR}/dishtiny* . # copy over executable

echo "   PWD" $PWD

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

export OMP_NUM_THREADS=2

./dishtiny -SEED $SEED -SEED_POP 1 -RUN_LENGTH 1 -SNAPSHOT_LENGTH 8192 -SNAPSHOT_FREQUENCY 1 \
  >"title=run+seed=${SEED}+ext=.log" 2>&1


################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
