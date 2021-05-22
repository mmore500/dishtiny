#!/bin/bash

################################################################################
echo
echo "running collate_genome_statistics_series.sh"
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

python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/genomes/stage=0~1+what=generated/stint=${stint}/" 'series=[0-9]+/a=genome_statistics\+criteria=.*\+morph=.*\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+variation=master\+ext=\.csv' \
  || python3 "${script_dir}/../../postprocessing/collate.py" "s3://${bucket}/endeavor=${endeavor}/genomes/stage=0+what=generated/stint=${stint}/" 'series=[0-9]+/a=genome_statistics\+criteria=.*\+morph=.*\+proc=[0-9]+\+series=[0-9]+\+stint=[0-9]+\+thread=[0-9]+\+variation=master\+ext=\.csv' \
  || echo "genome statistics collation across series failed"
