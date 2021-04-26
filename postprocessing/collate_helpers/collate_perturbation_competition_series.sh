#!/bin/bash

################################################################################
echo
echo "running collate_perturbation_competition_series.sh"
echo "-------------------------------------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 5 )); then
  echo "USAGE: [bucket] [endeavor] [stint] [extrospective/introspective/writable] [exchange/rotate]"
  exit 1
fi

bucket="$1"
echo "bucket ${bucket}"
shift

endeavor="$1"
echo "endeavor ${endeavor}"
shift

stint="$1"
echo "stint ${stint}"
shift

state_target="$1"
echo "state_target ${state_target}"
shift

perturbation="$1"
echo "perturbation ${perturbation}"
shift

################################################################################
echo
echo "other initialization"
echo "--------------------"
################################################################################

[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# adapted from https://stackoverflow.com/a/24114056
script_dir="$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")"
echo "script_dir ${script_dir}"

################################################################################
echo
echo "do collation"
echo "------------"
################################################################################

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/perturbation-${state_target}-state-${perturbation}-competitions/stage=1+what=generated/stint=${stint}/" 'a=coalescence_result\+proc=[0-9]+\+replicate=[0-9]+\+.*\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "perturbation competition collation across series failed"
