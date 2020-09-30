#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-630
#SBATCH --mem=12G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 2
#SBATCH --job-name freeforall
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
read -r REP SEED_A SEED_B <<< $(python3 -c "
import itertools as it
a = [1001, 1002, 1008, 1049, 1015, 1040, 1048, ]
combos = list(it.combinations(a, 2))
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

OUTPUT_DIR="/mnt/scratch/mmore500/dishtiny-freeforall/seed_a=${SEED_A}+seed_b=${SEED_B}+rep=${REP}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/freeforall/"
SOURCE_DIR_A=$(                                                                \
  ls -vd "/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_A}+step="*         \
  | tail -n 2                                                                  \
  | head -n 1                                                                  \
)
SOURCE_DIR_B=$(                                                                \
  ls -vd "/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED_B}+step="*         \
  | tail -n 2                                                                  \
  | head -n 1                                                                  \
)

echo "   SEED_A" $SEED_A
echo "   SEED_B" $SEED_B
echo "   REP" $REP
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
POP_PATHS_A=$(echo "${SOURCE_DIR_A}/"*".json.cereal")
POP_PATHS_B=$(echo "${SOURCE_DIR_B}/"*".json.cereal")

echo "   POP_PATHS_A" $POP_PATHS_A
echo "   POP_PATHS_B" $POP_PATHS_B

for POP_PATH_A in $POP_PATHS_A; do

  # get seed population from preceeding run
  POP_FILENAME_A=$(basename ${POP_PATH_A})

  echo "   POP_PATH_A" $POP_PATH_A
  echo "   POP_FILENAME_A" $POP_FILENAME_A

  # copy over competitors
  cp $POP_PATH_A "seedpop/id=1+${POP_FILENAME_A}"

done

for POP_PATH_B in $POP_PATHS_B; do

  # get seed population from preceeding run
  POP_FILENAME_B=$(basename ${POP_PATH_B})

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

export OMP_NUM_THREADS=2

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
