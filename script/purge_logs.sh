#!/bin/bash

################################################################################
echo
echo "running purge_logs.sh"
echo "---------------------"
################################################################################

if (( "$#" != 1 )); then
  echo "USAGE: [num_days_to_keep]"
  exit 1
fi

num_days="${1}"
echo "num_days ${num_days}"
shift

find /mnt/home/$(whoami)/slurmlogs -mtime "+${num_days}" -delete
find /mnt/home/$(whoami)/slurmscripts -mtime "+${num_days}" -delete
find /mnt/scratch/$(whoami)/slurmlogs -mtime "+${num_days}" -delete
find /mnt/scratch/$(whoami)/slurmscripts -mtime "+${num_days}" -delete
