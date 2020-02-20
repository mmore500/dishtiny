#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-440
#SBATCH --mem=16G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 1
#SBATCH --job-name match-screen-groups
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"
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
SEED=$(( SEED_OFFSET + SLURM_ARRAY_TASK_ID ))
read -r SEED_A SEED_B GROUP <<< $(python3 -c "
import itertools as it
import random
random.seed(1)

has = [1000, 1012, 1015, 1018, 1024, 1031, 1045, 1047, 1056, 1059, 1060,]
hasnot = [
  x for x in range(${SEED_OFFSET}, ${SEED_OFFSET} + 64)
  if x not in has
][:len(has)]
assert(len(has) == len(hasnot))

measuring_stick = random.sample(
  list(range(${SEED_OFFSET}, ${SEED_OFFSET} + 64)),
  20,
)

trials = (
  list(it.product(has, measuring_stick))
  + list(it.product(hasnot, measuring_stick))
)

ax, bx = next(
  trial for i, trial in enumerate(trials) if i == ${SLURM_ARRAY_TASK_ID}
)
print(
  ax,
  bx,
  'has' if ax in has else 'hasnot' if ax in hasnot else 'unknown'
)
")

OUTPUT_DIR="/mnt/scratch/mmore500/match-screen-groups/group=${GROUP}+seed_a=${SEED_A}+seed_b=${SEED_B}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/match-screen-groups/"
SOURCE_DIR_A="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_A}+step=1049"
SOURCE_DIR_B="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_B}+step=1049"

echo "   GROUP" $GROUP
echo "   SEED_A" $SEED_A
echo "   SEED_B" $SEED_B
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR
echo "   SOURCE_DIR_A" $SOURCE_DIR_A
echo "   SOURCE_DIR_B" $SOURCE_DIR_B

export SLURM_LOGFILE="slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out"
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
cp -r ${CONFIG_DIR}/* ${OUTPUT_DIR}
cd ${OUTPUT_DIR}
echo "   PWD" $PWD

# prepare python environment
module purge; module load GCC/7.3.0-2.30 OpenMPI/3.1.1 Python/3.6.6
source "/mnt/home/mmore500/myPy/bin/activate"

# get seed population from preceeding run
mkdir seedpop
# cut to just use dominant from first subpopulation...
POP_PATHS_A=$(                                                                 \
  echo "${SOURCE_DIR_A}/"*".json"                                              \
  | cut -d " " -f1                                                             \
)
POP_PATHS_B=$(                                                                 \
  echo "${SOURCE_DIR_B}/"*".json"                                              \
  | cut -d " " -f1                                                             \
)

echo "   POP_PATHS_A" $POP_PATHS_A
echo "   POP_PATHS_B" $POP_PATHS_B

for POP_PATH_A in $POP_PATHS_A; do

  # get seed population from preceeding run
  POP_FILENAME_A=$(                                                            \
    basename ${POP_PATH_A}                                                     \
    | sed "s/count=[[:digit:]]\++/count=1+/g"                                  \
  )

  echo "   POP_PATH_A" $POP_PATH_A
  echo "   POP_FILENAME_A" $POP_FILENAME_A

  # copy over competitors
  cp $POP_PATH_A "seedpop/id=1+${POP_FILENAME_A}.cereal"

done

for POP_PATH_B in $POP_PATHS_B; do

  # get seed population from preceeding run
  POP_FILENAME_B=$(                                                            \
    basename ${POP_PATH_B}                                                     \
    | sed "s/count=[[:digit:]]\++/count=1+/g"                                  \
  )

  echo "   POP_PATH_B" $POP_PATH_B
  echo "   POP_FILENAME_B" $POP_FILENAME_B

  # copy over competitors
  cp $POP_PATH_B "seedpop/id=2+${POP_FILENAME_B}"

done

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

export OMP_NUM_THREADS=1

for EVAL in {0..3}; do

  for CC in {0..0}; do

    ./dishtiny                                                                 \
      -SEED $(( ${SEED} + ${EVAL} * 4 + ${CC} + 100 ))                         \
      -SEED_POP $(( ${CC} + ${EVAL} * 4 + 1))                                  \
      >"title=run+EVAL=${EVAL}+CC=${CC}+ext=.log" 2>&1 &

  done

  wait

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
