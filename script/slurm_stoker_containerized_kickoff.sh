#!/bin/bash

################################################################################
echo
echo "running slurm_stoker_containerized_kickoff.sh"
echo "---------------------------------------------"
################################################################################

# fail on error
set -e

if (( "$#" != 3 )); then
  echo "USAGE: [bucket] [container_tag] [repo_sha]"
  echo "run this inside of a directory containing all the *.slurm.sh jobs"
  echo "you want to queue up"
  exit 1
fi

container_tag="${1}"
shift

export repo_sha="${1}"
shift

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

################################################################################
echo
echo "Download slurm stoker job template"
echo "-----------------------------------"
################################################################################

JOB_TEMPLATE="$(mktemp)"
JOB_SCRIPT="$(mktemp)"

echo "JOB_TEMPLATE ${JOB_TEMPLATE}"
echo "JOB_SCRIPT ${JOB_SCRIPT}"

for retry in {1..20}; do
  curl \
    -o "${JOB_TEMPLATE}" \
    "https://raw.githubusercontent.com/mmore500/dishtiny/${repo_sha}/script/slurm_stoker.slurm.sh.jinja" \
  && echo "  job template curl success" && break \
  || (echo "retrying job template curl (${retry})" && sleep $((RANDOM % 10)))
  if ((${retry}==20)); then echo "job template curl fail" && exit 123123; fi
done

################################################################################
echo
echo "Zip *.slurm.sh files into payload"
echo "and instantiate slurm stoker job template"
echo "-------------------------------------"
################################################################################

pwd
echo "$(ls *.slurm.sh | wc -l) target files detected"

tar -czvf - *.slurm.sh \
| uuencode -m - \
| python3 -c "

import json
import sys

print( json.dumps( {
  'payload' : sys.stdin.read(),
  'bucket' : '${bucket}',
  'container_tag' : '${container_tag}',
  'repo_sha' : '${repo_sha}',
} ) )

" | j2 --format=json -o "${JOB_SCRIPT}" "${JOB_TEMPLATE}" -

echo "job script instantiation size $(du -h "${JOB_SCRIPT}")"

################################################################################
echo
echo "Submit slurm stoker job"
echo "--------------------------------"
################################################################################

# adapted from https://superuser.com/a/689340
# and https://stackoverflow.com/a/4642975
host_sbatch_url="https://raw.githubusercontent.com/mmore500/dishtiny/${ repo_sha}/script/host_sbatch.sh"
echo "host_sbatch_url ${host_sbatch_url}"
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "${host_sbatch_url}" \
  | bash -s "${JOB_SCRIPT}" \
  && rm -f "${JOB_SCRIPT}" \
  && echo "${JOB_SCRIPT} submitted"


################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
