#!/bin/bash

################################################################################
echo
echo "running postprocess_all_stints.sh"
echo "-----------------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "parse command line arguments"
echo "----------------------------"
################################################################################

if (( "$#" < 3 )); then
  echo "USAGE: [bucket] [endeavor] [stints...]"
  exit 1
fi

bucket="$1"
echo "bucket ${bucket}"
shift

endeavor="$1"
echo "endeavor ${endeavor}"
shift

stints="$@"
echo "stints ${stints}"
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
echo "do postprocessing"
echo "-----------------"
################################################################################

for stint in ${stints}; do
  "${script_dir}/postprocess_one_stint.sh" "${bucket}" "${endeavor}" "${stint}"
done

# series profiles
"${script_dir}/collate_series_profiles_across_stints.sh" "${bucket}" "${endeavor}"

"${script_dir}/elaborate_all_stints_all_series_profiles.py" "${bucket}" "${endeavor}"

# thread profiles
"${script_dir}/collate_thread_profiles_across_stints.sh" "${bucket}" "${endeavor}"

"${script_dir}/elaborate_all_stints_all_thread_profiles.py" "${bucket}" "${endeavor}"
