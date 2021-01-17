#!/bin/bash

# fail on error
set -e

if (( "$#" < 4 )); then
  echo "USAGE: [container_tag] [repo_sha] [stint] [series...]"
  exit 1
fi

CONTAINER_TAG="${1}"
shift

REPO_SHA="${1}"
shift

STINT="${1}"
shift

SERIES="${@}"

JOB_TEMPLATE="$(mktemp)"
curl -o "${JOB_TEMPLATE}" "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/slurm/evolve/evolvejob.slurm.sh.jinja"

source ~/pyenv/bin/activate || :

for just_one_series in "${SERIES}"; do

  JOB_SCRIPT="$(mktemp)"

  echo "series ${just_one_series}"
  echo "JOB_SCRIPT ${JOB_SCRIPT}"

  j2 --format=yaml -o "${JOB_SCRIPT}" "${JOB_TEMPLATE}" << J2_HEREDOC_EOF
container_sha: "${CONTAINER_SHA}"
repo_sha: "${REPO_SHA}"
series: "${just_one_series}"
stint: "${STINT}"
J2_HEREDOC_EOF

  for retry in {1..10}; do
    sbatch "${JOB_SCRIPT}" && echo "  job submit success" && break \
      || (echo "retrying job submit (${retry})" && sleep $((RANDOM % 10)))
    if ((${retry}==10)); then echo "job submit fail" && exit 1; fi
  done &

done
