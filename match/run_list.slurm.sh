#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-79
#SBATCH --mem=16G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 4
#SBATCH --job-name match
#SBATCH --account=devolab
#SBATCH --output="/mnt/home/mmore500/slurmlogs/slurm-%A_%a.out"

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
read -r REP SEED_A SEED_B <<< $(python3 -c "
import itertools as it
a = [1004, 1017, 1029, 1051, 1071, 1078, 1083, 1085, ]
b = [
  x for x in range(${SEED_OFFSET}, ${SEED_OFFSET} + 100)
  if x not in a
]
combos = [
  (ax, bx)
  for rot_a in ( a[n:] + a[:n] for n in range(len(a)) )
  for ax, bx in zip(it.cycle(rot_a), b)
]
repped_combos = (
  (rep, combo)
  for rep, combos in enumerate(it.repeat(combos))
  for combo in combos
)
rep, (ax, bx) = next(
  x for i, x in enumerate(repped_combos) if i == ${SLURM_ARRAY_TASK_ID}
)
print(rep, ax, bx)
")
LAST_STEP=24

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-match/seed_a=${SEED_A}+seed_b=${SEED_B}+rep=${REP}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/match/"
SOURCE_DIR_A="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_A}+step=${LAST_STEP}"
SOURCE_DIR_B="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_B}+step=${LAST_STEP}"

echo "   SEED" $SEED_A
echo "   SEED" $SEED_B
echo "   REP" $REP
echo "   LAST_STEP" $LAST_STEP
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
POP_PATH_A=$(echo "${SOURCE_DIR_A}/"*".json.cereal")
POP_FILENAME_A=$(basename ${POP_PATH_A})

# get seed population from preceeding run
POP_PATH_B=$(echo "${SOURCE_DIR_B}/"*".json.cereal")
POP_FILENAME_B=$(basename ${POP_PATH_B})

echo "   POP_PATH_A" $POP_PATH_A
echo "   POP_FILENAME_A" $POP_FILENAME_A

echo "   POP_PATH_B" $POP_PATH_B
echo "   POP_FILENAME_B" $POP_FILENAME_B

# copy over competitors
mkdir seedpop
cp $POP_PATH_A "seedpop/id=1+${POP_FILENAME_A}"
cp $POP_PATH_B "seedpop/id=2+${POP_FILENAME_B}"

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

./dishtiny -SEED $(( ${SEED} )) -SEED_POP 1 >run.log 2>&1

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
