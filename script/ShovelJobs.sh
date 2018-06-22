#!/bin/sh

# we will shovel successfully completed jobs into this directory
mkdir -p confirmed

# launch 250 parallel job submissions
for f in $(ls | head -250);
  # only move jobs to confirmed directory on successful submission
  do (python /mnt/home/mmore500/dist_qsub/dist_qsub.py $f --nocheckpoint && mv $f confirmed && echo $f "success") & done
