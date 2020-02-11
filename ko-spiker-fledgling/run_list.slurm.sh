#!/bin/bash
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --array=0-63
#SBATCH --mem=24G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 8
#SBATCH --job-name ko-spiker-fledgling
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
SEED=$((SLURM_ARRAY_TASK_ID + SEED_OFFSET))

OUTPUT_DIR="/mnt/scratch/mmore500/ko-spiker-fledgling/seed=${SEED}"
CONFIG_DIR="/mnt/home/mmore500/dishtiny/ko-spiker-fledgling/"
# get second most recent source population
SOURCE_DIR=$(                                                                  \
  ls -vd "/mnt/scratch/mmore500/dishtiny-screen/seed=${SEED}+step="*           \
  | tail -n 2                                                                  \
  | head -n 1                                                                  \
)

echo "   SEED" $SEED
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

# extract zipped population files
for ZIPPED_PATH in "${SOURCE_DIR}/"*".json.cereal.tar.gz"; do
  echo "   ZIPPED_PATH" $ZIPPED_PATH
  tar -C ${SOURCE_DIR} -xf $ZIPPED_PATH &
done

wait

# get seed population from preceeding run
POP_PATHS=$(echo "${SOURCE_DIR}/"*".json.cereal")
mkdir seedpop

echo "   POP_PATHS" $POP_PATHS

for POP_PATH in $POP_PATHS; do

  POP_FILENAME=$(basename ${POP_PATH})
  KO_PATH="seedpop/id=2+${POP_FILENAME}"

  echo "   POP_PATH" $POP_PATH
  echo "   POP_FILENAME" $POP_FILENAME
  echo "   KO_PATH" $KO_PATH

  # copy over WT and to-be KO populations
  cp $POP_PATH "seedpop/id=1+${POP_FILENAME}"
  cp $POP_PATH $KO_PATH

  # split into chunks to do knockouts on individual genome components
    # 0th chunk: nada (ok to still sed it though!)
    # odd chunks: pointer
    # even chunks: spiker
  csplit --suffix-format="%09d" ${KO_PATH} '/program.*{$/' '{*}'               \
    > /dev/null # ignore byte counts printed to stdout

  # knockout pointer components, genome by genome
  # 27,Nop
  # 121,TryAddFledglingConnection

  for f in xx*1 xx*3 xx*5 xx*7 xx*9; do
    sed -i -- "s/\"id\": 121\$/\"id\": 27/g" $f &
    while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
  done

  # knockout spiker components, genome by genome
  # 27,Nop
  # 98,TryAddFledglingConnection

  for f in xx*0 xx*2 xx*4 xx*6 xx*8; do
    sed -i -- "s/\"id\": 98\$\/\"id\": 27/g" $f &
    while [ $(jobs -r | wc -l) -gt 100 ]; do sleep 1; done
  done

  # recombine components and delete fragments
  wait
  cat xx* > ${KO_PATH}
  rm xx*

  echo "   end loop"
  echo

done

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

module purge; module load GCC/8.2.0-2.31.1 OpenMPI/3.1.3 HDF5/1.10.4;

export OMP_NUM_THREADS=2

for REP in {0..3}; do

  for CC in {0..3}; do

    ./dishtiny                                                                 \
      -SEED $(( ${SEED} + ${REP} * 4 + ${CC} + 100 ))                          \
      -SEED_POP $(( ${REP} * 4 + ${CC} + 1 ))                                  \
      >"title=run+REP=${REP}+CC=${CC}+ext=.log" 2>&1 &

  done

  wait

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
