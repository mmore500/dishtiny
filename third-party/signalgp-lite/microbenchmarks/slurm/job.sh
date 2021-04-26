#!/bin/zsh

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

export SLURM_LOGFILE="slurm-${SLURM_JOB_ID}.out"
export SLURM_LOGPATH="/mnt/home/mmore500/slurmlogs/${SLURM_LOGFILE}"

echo "   SLURM_LOGFILE" $SLURM_LOGFILE
echo "   SLURM_LOGPATH" $SLURM_LOGPATH

################################################################################
echo
echo "Setup Work Dir"
echo "--------------"
################################################################################

cd $(mktemp -d)
echo "   PWD" $PWD

################################################################################
echo
echo "Do Work"
echo "-------"
################################################################################

singularity run docker://mmore500/conduit /bin/bash -c "cp -r /opt/conduit/* . && make microbenchmark" > logfile.txt

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
