#!/bin/bash

# fail on error
set -e

################################################################################
echo
echo "running host_sbatch.sh"
echo "-------------------------------"
################################################################################

JOB_SCRIPT="${1}"
echo "JOB_SCRIPT ${JOB_SCRIPT}"

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

for retry in {0..20}; do

  if ((${retry}==20)); then echo "  job submit fail" && exit 123123; fi

  sshpass -p "${HOST_PASSWORD}" \
    scp -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${JOB_SCRIPT}" "${HOST_USERNAME}@$(hostname):${JOB_SCRIPT}" \
  && echo "  job script copy success" \
  || (echo "retrying job script copy (${retry})" && sleep $((RANDOM % 10)) && continue)

  echo "JOB_ID=\"\$(sbatch --parsable \"${JOB_SCRIPT}\")\"; cp \"${JOB_SCRIPT}\" \$(realpath ~)/slurmscripts/slurm-\${JOB_ID}.sh && echo \"submitted batch job \${JOB_ID}\"" \
  | sshpass -p "${HOST_PASSWORD}" \
    ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${HOST_USERNAME}@$(hostname)" -X \
      'bash -login'  \
  && echo "  job submit success" && break \
  || (echo "retrying job submit (${retry})" && sleep $((RANDOM % 10)) && continue)

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
