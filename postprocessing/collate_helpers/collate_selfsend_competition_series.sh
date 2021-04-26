#!/bin/bash

################################################################################
echo
echo "running collate_selfsend_competition_series.sh"
echo "-------------------------------------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 4 )); then
  echo "USAGE: [bucket] [endeavor] [stint] [intra/inter]"
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

messaging_target="$1"
echo "messaging_target ${messaging_target}"
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

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/selfsend-${messaging_target}-competitions/stage=1+what=generated/stint=${stint}/" 'a=coalescence_result\+proc=[0-9]+\+replicate=[0-9]+\+.*\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "selfsend competition collation across series failed"
