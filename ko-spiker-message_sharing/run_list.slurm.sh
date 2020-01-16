#!/bin/zsh
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

OUTPUT_DIR="/mnt/scratch/mmore500/ko-spiker-message_sharing/seed=${SEED}+rep=${REP}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/ko-spiker-message_sharing/"
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

rm -rf ${OUTPUT_DIR}/* || echo "   not a redo"  #If this is a redo, clear the dir
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


FILE_LENGTH=$(cat ${KO_PATH} | wc -l)
echo "   FILE_LENGTH" $FILE_LENGTH

# knockout spiker components, genome by genome
# 27,Nop
# 80,SendBigFracResource
# 81,SendSmallFracResource
# 101,SendSpikeMsg

for START STOP in                                                              \
  $(sed -n '/program.*{$/=' ${KO_PATH} | tail -n +2) $FILE_LENGTH              \
; do
  let ++REGION_COUNTER
  for TARGET in 80 81 101; do
    let ++OPERATION_COUNTER
    SED_POINTER_COMMAND+="${START},${STOP}s/\"id\": ${TARGET}\$/\"id\": 27/g; "
  done
done

echo "   REGION_COUNTER" $REGION_COUNTER
echo "   OPERATION_COUNTER" $OPERATION_COUNTER
echo "   SED_POINTER_COMMAND" $SED_POINTER_COMMAND | head -c 100 && echo

sed -i -- $SED_POINTER_COMMAND $KO_PATH

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
