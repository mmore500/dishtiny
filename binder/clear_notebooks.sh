#!/bin/bash

################################################################################
echo
echo "running clear_notebooks.sh"
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
echo "clear notebooks in current directory"
echo "--------------------------------------"
################################################################################

shopt -s nullglob

for notebook in "${script_dir}/"*.ipynb; do
  jupyter nbconvert --clear-output --inplace "${notebook}"
done

shopt -u nullglob

################################################################################
echo
echo "recurse to subdirectories"
echo "-------------------------"
################################################################################

shopt -s nullglob

for script in "${script_dir}/"*/clear_notebooks.sh; do
  "${script}"
done

shopt -u nullglob
