#!/bin/bash

VERSION=$(git describe --dirty --always)
TIME=$(date +%s)
NNODE=$([ -z "$SLURM_JOB_NUM_NODES" ] && echo 1 || echo $SLURM_JOB_NUM_NODES)
NPROC=$(nproc)

OSF_PATH="microbenchmarks/context=${CONTEXT}/nnode=${NNODE}+nproc=${NPROC}/time=${TIME}+version=${VERSION}"

set -o xtrace

for f in $(find . -type f -name "*.json" -o -name "*.png" -o -name "*.csv"); do
  #TODO fixme, currently broken by server issues on osf's end
  #osf -u $OSF_USERNAME -p 3v9kp upload $f $OSF_PATH/$f;
  true
done
