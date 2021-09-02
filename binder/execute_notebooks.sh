#!/bin/bash

################################################################################
echo
echo "running execute_notebooks.sh"
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
echo "execute notebooks in current directory"
echo "--------------------------------------"
################################################################################

shopt -s nullglob

for notebook in "${script_dir}/"*.ipynb; do
  echo "notebook ${notebook}"
  export NOTEBOOK_NAME="$(basename "${notebook%.*}")"
  export NOTEBOOK_PATH="$(realpath "${notebook}")"
  jupyter nbconvert \
    --to notebook --execute --inplace \
    --ExecutePreprocessor.timeout=600 \
    "${notebook}"
done

shopt -u nullglob

################################################################################
echo
echo "upload outplots in current directory"
echo "--------------------------------------"
################################################################################

if test -d "${script_dir}/outplots/" && [[ "${1}" == "upload" ]]; then

shopt -s nullglob
cd "${script_dir}/outplots/"

for outplot in *{.pdf,.png}; do
  keyname stash --move --drop "${outplot}" _dfdigest
done

for outfile in *{.pdf,.png}*; do

  bucket="$(echo "${outfile}" | keyname extract bucket)"
  endeavor="$(echo "${outfile}" | keyname extract endeavor)"
  echo "bucket ${bucket}"
  echo "endeavor ${endeavor}"

  aws s3 cp --quiet \
    "${outfile}" \
    "s3://${bucket}/endeavor=${endeavor}/stage=9+what=plotted/${outfile}" \
    & pids+=($!)

    # limit to twenty concurrent upload jobs
    while (( $(jobs -p | wc -l) > 20 )); do sleep 1; done
done

# wait on all forked upload jobs
for pid in "${pids[@]}"; do
  # if child process fails, we fail
  wait "${pid}"
done

unset pids

cd -
shopt -u nullglob

fi # if test -d "${script_dir}/outplots/" && [[ "${1}" == "upload" ]]; then

################################################################################
echo
echo "upload notebooks in current directory"
echo "--------------------------------------"
################################################################################

if [[ "${1}" == "upload" ]]; then

shopt -s nullglob
cd "${script_dir}"

bucket="$(pwd | sed 's/\//+/g' - | keyname extract bucket)"
endeavor="$(pwd | sed 's/\//+/g' - | keyname extract endeavor)"
echo "bucket ${bucket}"
echo "endeavor ${endeavor}"

test -n "${bucket}" && test -n "${endeavor}" && for outfile in *.ipynb; do

  aws s3 cp --quiet \
    "${outfile}" \
    "s3://${bucket}/endeavor=${endeavor}/stage=9+what=notebooks/${outfile}" \
    & pids+=($!)

    # limit to twenty concurrent upload jobs
    while (( $(jobs -p | wc -l) > 20 )); do sleep 1; done
done

# wait on all forked upload jobs
for pid in "${pids[@]}"; do
  # if child process fails, we fail
  wait "${pid}"
done

unset pids

cd -
shopt -u nullglob

fi # if test -d "${script_dir}/outplots/" && [[ "${1}" == "upload" ]]; then

################################################################################
echo
echo "recurse to subdirectories"
echo "-------------------------"
################################################################################

shopt -s nullglob

for script in "${script_dir}/"*/execute_notebooks.sh; do
  "${script}" "$@"
done

shopt -u nullglob
