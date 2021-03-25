#!/bin/bash

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

################################################################################
echo
echo "running host_sbatch.sh"
echo "-------------------------------"
################################################################################

JOB_SCRIPT="${1}"
echo "JOB_SCRIPT ${JOB_SCRIPT}"
echo "realpath JOB_SCRIPT $(realpath "${JOB_SCRIPT}")"

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

for retry in {0..20}; do

  if ((${retry}==20)); then echo "  job submit fail" && exit 123123; fi

  sshpass -p "${HOST_PASSWORD}" \
    scp -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${JOB_SCRIPT}" "${HOST_USERNAME}@$(hostname):$(realpath "${JOB_SCRIPT}")" \
  && echo "  job script copy success" \
  || (echo "retrying job script copy (${retry})" && sleep $((RANDOM % 10)) && continue)

  echo "JOB_ID=\"\$(sbatch --parsable \"$(realpath "${JOB_SCRIPT}")\")\"; mkdir -p \"/mnt/scratch/\$(whoami)/slurmscripts/\"; cp \"$(realpath "${JOB_SCRIPT}")\" \"/mnt/scratch/\$(whoami)/slurmscripts/slurm-\${JOB_ID}.sh\" && echo \"submitted batch job \${JOB_ID}\"" \
  | sshpass -p "${HOST_PASSWORD}" \
    ssh -q -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "${HOST_USERNAME}@$(hostname)" -X \
      'bash'  \
  && echo "  job submit success" && break \
  || (echo "retrying job submit (${retry})" && sleep $((RANDOM % 10)) && continue)

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################
