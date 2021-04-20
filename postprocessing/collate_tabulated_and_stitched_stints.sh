#!/bin/bash

################################################################################
echo
echo "running tabulated_and_stitched_stints.sh"
echo "----------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 2 )); then
  echo "USAGE: [bucket] [endeavor]"
  exit 1
fi

bucket="$1"
echo "bucket ${bucket}"
shift

endeavor="$1"
echo "endeavor ${endeavor}"
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

python3 postprocessing/collate.py "s3://${bucket}/endeavor=${endeavor}/stage=4+what=tabulated_and_stitched/" 'a=series_profiles\+stint=[0-9]+\+ext=\.csv.xz' \
|| echo "tabulated_and_stitched collation across stints failed"
