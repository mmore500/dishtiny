#!/bin/bash

################################################################################
echo
echo "running slurm_stoker_kickoff.sh"
echo "-------------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" != 4 )); then
  echo "USAGE: [bucket] [container_tag] [repo_sha] [title]"
  echo "run this inside of a directory containing all the *.slurm.sh jobs"
  echo "you want to queue up"
  exit 1
fi

bucket="${1}"
echo "bucket ${bucket}"
shift

container_tag="${1}"
echo "container_tag ${container_tag}"
shift

repo_sha="${1}"
echo "repo_sha ${repo_sha}"
shift

title="${1}"
echo "title ${title}"
shift

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# load pyenv, if available
[[ -f ~/pyenv/bin/activate ]] && source ~/pyenv/bin/activate || echo "no pyenv"

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

echo "$(ls *.slurm.sh | wc -l) target files detected"

tar -czvf - *.slurm.sh \
| singularity exec "docker://mmore500/sharutils:sha-223389d" uuencode -m - \
| python3 -c "

import json
import sys

print( json.dumps( {
  'payload' : sys.stdin.read(),
  'bucket' : '${bucket}',
  'container_tag' : '${container_tag}',
  'num_jobs' : '$(ls *.slurm.sh | wc -l)',
  'repo_sha' : '${repo_sha}',
  'title' : '${title}',
} ) )

" | j2 --format=json -o "${JOB_SCRIPT}" "${JOB_TEMPLATE}" -

################################################################################
echo
echo "Submit slurm stoker job"
echo "--------------------------------"
################################################################################

JOB_ID="$(sbatch --parsable "${JOB_SCRIPT}")" \
  cp "${JOB_SCRIPT}" ~/slurmlogs/slurm-${JOB_ID}.sh

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
