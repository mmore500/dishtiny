#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-999
#SBATCH --mem=16G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 4
#SBATCH --job-name ko-all-regulation
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
SEED=$((SLURM_ARRAY_TASK_ID / 10 + SEED_OFFSET))
REP=$((SLURM_ARRAY_TASK_ID % 10))
LAST_STEP=24

OUTPUT_DIR="/mnt/scratch/mmore500/ko-all-regulation/seed=${SEED}+rep=${REP}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/ko-all-regulation/"
SOURCE_DIR="/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED}+step=${LAST_STEP}"

echo "   SEED" $SEED
echo "   REP" $REP
echo "   LAST_STEP" $LAST_STEP
echo "   OUTPUT_DIR" $OUTPUT_DIR
echo "   CONFIG_DIR" $CONFIG_DIR
echo "   SOURCE_DIR" $SOURCE_DIR

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
MAX_UPDATE=$(python3 -c "import sys; from keyname import keyname as kn; print( \
  max(                                                                         \
    int(kn.unpack(filename)['update'])                                         \
    for filename in sys.argv[1:]                                               \
  )                                                                            \
)" "${SOURCE_DIR}/"*".json.cereal")
POP_PATH=$(echo "${SOURCE_DIR}/"*"+update=${MAX_UPDATE}+"*".json.cereal")
POP_FILENAME=$(basename ${POP_PATH})
KO_PATH="seedpop/id=2+${POP_FILENAME}"

echo "   MAX_UPDATE" $MAX_UPDATE
echo "   POP_PATH" $POP_PATH
echo "   POP_FILENAME" $POP_FILENAME
echo "   KO_PATH" $KO_PATH

# copy over WT and to-be KO populations
mkdir seedpop
cp $POP_PATH "seedpop/id=1+${POP_FILENAME}"
cp $POP_PATH $KO_PATH

# split into chunks to do knockouts on individual genome components
  # 0th chunk: nada (ok to still sed it though!)
  # odd chunks: pointer
  # even chunks: spiker
csplit --suffix-format="%09d" ${KO_PATH} '/program.*{$/' '{*}'              \
  > /dev/null # ignore byte counts printed to stdout

# knockout pointer components, genome by genome
# 27,Nop
# 29,SetRegulator
# 30,SetOwnRegulator
# 31,AdjRegulator
# 32,AdjOwnRegulator
# 66,DuplicateSetRegulator
# 67,DuplicateSetOwnRegulator
# 68,DuplicateExtRegulator

for f in xx*1 xx*3 xx*5 xx*7 xx*9; do
  sed -i -- "s/\"id\": 29\$\|\"id\": 30\$\|\"id\": 31\$\|\"id\": 32\$\|\"id\": 66\$\|\"id\": 67\$\|\"id\": 68\$/\"id\": 27/g" $f &
  while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
done

# knockout spiker components, genome by genome
# 27,Nop
# 29,SetRegulator
# 30,SetOwnRegulator
# 31,AdjRegulator
# 32,AdjOwnRegulator
# 66,DuplicateSetRegulator
# 67,DuplicateSetOwnRegulator
# 68,DuplicateAdjRegulator
# 69,DuplicateAdjOwnRegulator


for f in xx*0 xx*2 xx*4 xx*6 xx*8; do
  sed -i -- "s/\"id\": 29\$\|\"id\": 30\$\|\"id\": 31\$\|\"id\": 32\$\|\"id\": 66\$\|\"id\": 67\$\|\"id\": 68\$\|\"id\": 69\$/\"id\": 27/g" $f &
  while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
done

# recombine components and delete fragments
wait
cat xx* > ${KO_PATH}
rm xx*

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

./dishtiny -SEED $(( ${SEED} + ${REP} + 25 )) -SEED_POP 1 >run.log 2>&1

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
