#!/bin/bash

################################################################################
echo
echo "running collate_noncritical_phenotypeequivalent_nopinterpolation_competition_series.sh"
echo "------------------------------------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 3 )); then
  echo "USAGE: [bucket] [endeavor] [stint]"
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

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/noncritical-phenotypeneutral-nopinterpolation-competitions/stage=5+what=generated/stint=${stint}/" 'a=coalescence_result\+proc=[0-9]+\+replicate=[0-9]+\+.*\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "noncritical/phenotypeneutral nop interpolation competition collation across series failed"
