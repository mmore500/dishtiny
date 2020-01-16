#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-999
#SBATCH --mem=16G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 4
#SBATCH --job-name ko-spiker-targeting
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

OUTPUT_DIR="/mnt/scratch/mmore500/ko-spiker-targeting/seed=${SEED}+rep=${REP}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/ko-spiker-targeting/"
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

# split into chunks to do knockouts on individual genome components
  # 0th chunk: nada (ok to still sed it though!)
  # odd chunks: pointer
  # even chunks: spiker
csplit --suffix-format="%09d" ${KO_PATH} '/program.*{$/' '{*}'              \
  > /dev/null # ignore byte counts printed to stdout

# knockout spiker components, genome by genome
# 27,Nop
# 86,RemoveOutgoingConnection
# 87,RemoveIncomingConnection
# 88,AddDevoUpQuery
# 89,AddDevoDownQuery
# 91,SetConnectionAgingParam
# 92,SetConnectionExploitParam
# 93,SetConnectionDevelopmentParam
# 94,SetConnectionSensingParam
# 98,PutMembraneBringer
# 99,PutMembraneBlocker
# 100,SetMembraneRegulator

for f in xx*0 xx*2 xx*4 xx*6 xx*8; do
  sed -i -- "s/\"id\": 80\$\|\"id\": 81\$\|\"id\": 101\$\|\"id\": 86\$\|\"id\": 87\$\|\"id\": 88\$\|\"id\": 89\$\|\"id\": 91\$\|\"id\": 92\$\|\"id\": 93\$\|\"id\": 94\$\|\"id\": 98\$\|\"id\": 99\$\|\"id\": 100\$/\"id\": 27/g" $f &
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
