#!/bin/bash

################################################################################
echo
echo "running clear_outplots.sh"
echo "---------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "other initialization"
echo "--------------------"
################################################################################

[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# adapted from https://stackoverflow.com/a/24114056
script_dir="$(dirname -- "$BASH_SOURCE")"
echo "script_dir ${script_dir}"

################################################################################
echo
echo "clear outplots in current directory"
echo "-----------------------------------"
################################################################################

rm -rf "${script_dir}/outplots"
rm -rf "${script_dir}/teeplots"

################################################################################
echo
echo "recurse to subdirectories"
echo "-------------------------"
################################################################################

shopt -s nullglob

for script in "${script_dir}/"*/clear_outplots.sh; do
  "${script}"
done

shopt -u nullglob
