#!/bin/bash

################################################################################
echo
echo "running collate_monoculture_runninglogs_series.sh"
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

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/monocultures/runninglogs/stage=1+what=generated/stint=${stint}/" 'series=[0-9]+/a=birth_log\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv.xz' \
  || echo "birth log collation across series failed"

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/monocultures/runninglogs/stage=1+what=generated/stint=${stint}/" 'series=[0-9]+/a=death_log\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv.xz' \
  || echo "death log collation across series failed"

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/monocultures/runninglogs/stage=1+what=generated/stint=${stint}/" 'series=[0-9]+/a=spawn_log\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+ext=\.csv.xz' \
  || echo "spawn log collation across series failed"
