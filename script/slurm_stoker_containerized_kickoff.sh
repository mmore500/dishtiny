#!/bin/bash

################################################################################
echo
echo "running slurm_stoker_containerized_kickoff.sh"
echo "---------------------------------------------"
################################################################################

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

################################################################################
echo
echo "Zip *.slurm.sh files into payload"
echo "-----------------------------------"
################################################################################

echo "$(ls *.slurm.sh | wc -l) target files detected"

export payload="$(tar -czvf - *.slurm.sh | uuencode -m -)"

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

echo "job script instantiation size $(du -h "${JOB_SCRIPT}")"

################################################################################
echo
echo "Submit slurm stoker job"
echo "--------------------------------"
################################################################################

for retry in {0..20}; do

  if ((${retry}==20)); then echo "  job script submit fail" && exit 123123; fi

  sshpass -p "${HOST_PASSWORD}" \
    scp -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${JOB_SCRIPT}" "${HOST_USERNAME}@$(hostname):${JOB_SCRIPT}" \
  && echo "  job script copy success" \
  || (echo "retrying job script copy (${retry})" && sleep $((RANDOM % 10)) && continue)

  echo "sbatch \"${JOB_SCRIPT}\"" \
  | sshpass -p "${HOST_PASSWORD}" \
    ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${HOST_USERNAME}@$(hostname)" -X \
      'bash -login'  \
  && echo "  job submit success" && break \
  || (echo "retrying job submit (${retry})" && sleep $((RANDOM % 10)))

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
