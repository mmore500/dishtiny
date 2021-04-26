#!/bin/bash

################################################################################
echo
echo "running collate_evolve_statistics_series.sh"
echo "---------------------------------------------"
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

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/evolve/metrics/stage=0+what=generated/stint=${stint}/" 'series=[0-9]+/a=demographic_phenotypic_phylogenetic_metrics\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "dppm statistics collation across series failed"

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/evolve/metrics/stage=0+what=generated/stint=${stint}/" 'series=[0-9]+/a=kin_conflict_statistics\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "kin conflict statistics collation across series failed"

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/evolve/metrics/stage=0+what=generated/stint=${stint}/" 'series=[0-9]+/a=kin_conflict_by_replev_statistics\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv' \
  || echo "kin conflict by replev statistics collation across series failed"
