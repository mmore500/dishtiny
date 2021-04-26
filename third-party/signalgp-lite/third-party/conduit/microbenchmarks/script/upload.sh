#!/bin/bash

VERSION=$(git describe --dirty --always)
TIME=$(date +%s)
NNODE=$([ -z "$SLURM_JOB_NUM_NODES" ] && echo 1 || echo "$SLURM_JOB_NUM_NODES")
NPROC=$(nproc)

OSF_PATH="microbenchmarks/context=${CONTEXT}/nnode=${NNODE}+nproc=${NPROC}/time=${TIME}+version=${VERSION}"

set -o xtrace

for f in $(find . -name "*.json") *.csv *.png; do
  :
  # TODO
  #osf -u "$OSF_USERNAME" -p 7jkgp upload "$f" "${OSF_PATH}/${f}";
done
