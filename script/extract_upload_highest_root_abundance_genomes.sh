#!/bin/bash

################################################################################
echo
echo "running extract_upload_highest_root_abundance_genomes.sh"
echo "--------------------------------------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" < 3 )); then
  echo "USAGE: [bucket] [series] [stints...]"
  exit 1
fi

BUCKET="${1}"
echo "BUCKET ${BUCKET}"
shift

SERIES="${1}"
echo "SERIES ${SERIES}"
shift

STINTS="${@}"
echo "STINTS ${STINTS}"
shift

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
echo "SCRIPT_DIR ${SCRIPT_DIR}"

ENDEAVOR="$(( SERIES / 1000 ))"
echo "ENDEAVOR ${ENDEAVOR}"

source ~/.secrets.sh

for STINT in ${STINTS}; do
  echo "STINT ${STINT}"

  # set up and jump into temporary work directory
  cd "$(mktemp -d)"

  POPULATION_URI="s3://${BUCKET}/endeavor=${ENDEAVOR}/evolve/populations/stage=0+what=generated/stint=${STINT}/series=${SERIES}/a=population+proc=0+series=${SERIES}+stint=${STINT}+thread=0+variation=master+ext=.bin.xz"
  aws s3 cp "${POPULATION_URI}" .

  "${SCRIPT_DIR}/../rundishtiny" -RUN 0 -GENESIS "reconstitute" -ARTIFACTS_DUMP 1 -SERIES "${SERIES}" -STINT "${STINT}"

  shopt -s nullglob

  echo "uploading any genome files..."
  for p in \
    outartifacts/*a=genome*criteria=abundance_highest_root* \
    outdata/*a=genome_statistics*criteria=abundance_highest_root* \
    outdata/*a=nopout_divergence_updates*criteria=abundance_highest_root* \
  ; do
    f="$(basename "${p}")"
    echo "uploading ${p} -> ${f}"
    for retry in {1..20}; do
      aws s3 cp --quiet \
        "${p}" \
        "s3://${BUCKET}/endeavor=${ENDEAVOR}/genomes/stage=1+what=extracted/stint=${STINT}/series=${SERIES}/${f}" \
      && echo "  ${f} upload success" && break \
      || (echo "retrying ${f} upload (${retry})" && sleep $((RANDOM % 10)))
      if ((${retry}==20)); then echo "$f upload fail" && exit 123123; fi
    done & pids+=($!)

    # limit to twenty concurrent upload jobs
    while (( $(jobs -p | wc -l) > 20 )); do sleep 1; done

  done
done

# wait on all forked upload jobs
for pid in "${pids[@]}"; do
  # if child process fails, we fail
  wait "${pid}"
done

unset pids
