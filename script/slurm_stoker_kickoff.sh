#!/bin/bash

# fail on error
set -e

if (( "$#" != 2 )); then
  echo "USAGE: [container_tag] [repo_sha]"
  echo "run this inside of a directory containing all the *.slurm.sh jobs"
  echo "you want to queue up"
  exit 1
fi

export container_tag="${1}"
shift

export repo_sha="${1}"
shift

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# load pyenv, if available
[[ -f ~/pyenv/bin/activate ]] && source ~/pyenv/bin/activate || echo "no pyenv"

################################################################################
echo
echo "Zip *.slurm.sh files into payload"
echo "-----------------------------------"
################################################################################

export payload="$(tar -czvf - *.slurm.sh | singularity exec "docker://mmore500/sharutils:sha-223389d" uuencode -)"

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
echo "Instantiate slurm stoker job template"
echo "-------------------------------------"
################################################################################

j2 --import-env "" -o "${JOB_SCRIPT}" "${JOB_TEMPLATE}" -

################################################################################
echo
echo "Submit slurm stoker job"
echo "--------------------------------"
################################################################################

sbatch "${JOB_SCRIPT}"

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
