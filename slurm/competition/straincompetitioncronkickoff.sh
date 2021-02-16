#!/bin/bash

################################################################################
echo
echo "running straincompetitioncronkickoff.sh"
echo "---------------------------------------"
################################################################################

# fail on error
set -e

if (( "$#" < 4 )); then
  echo "USAGE: [container_tag] [repo_sha] [stint] [series...]"
  exit 1
fi

CONTAINER_TAG="${1}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
shift

REPO_SHA="${1}"
echo "REPO_SHA ${REPO_SHA}"
shift

STINT="${1}"
echo "STINT ${STINT}"
shift

SERIES="${@}"
echo "SERIES ${SERIES}"

# set up and jump into temporary work directory
cd "$(mktemp -d)"

# curl repro_runner.sh script into to a temporary file
REPRO_RUNNER="$(mktemp)"
curl -o "${REPRO_RUNNER}" "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/script/repro_runner.sh"
chmod +x "${REPRO_RUNNER}"

################################################################################
echo
echo "Run Job with repro_runner.sh"
echo "--------------------------------"
################################################################################

"${REPRO_RUNNER}" \
  -p dnh2v -u mmore500 -s dishtiny \
  --repo_sha "${REPO_SHA}" --container_tag "${CONTAINER_TAG}" \
  << REPRO_RUNNER_HEREDOC_EOF

# fail on error
set -e

################################################################################
echo
echo "running straincompetitioncronkickoff.sh"
echo "---------------------------------------"
################################################################################

echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "STINT ${STINT}"
echo "SERIES ${SERIES}"

source ~/.secrets.sh || :

FIRST_SERIES=${SERIES%% *}
ENDEAVOR="\$(( FIRST_SERIES / 1000 ))"

echo "FIRST_SERIES \${SERIES}"
echo "ENDEAVOR \${ENDEAVOR}"

################################################################################
echo
echo "Generate Runscript"
echo "------------------"
################################################################################

JOB_SCRIPT="a=straincompetitioncron+ext=.slurm.sh"
echo "JOB_SCRIPT \${JOB_SCRIPT}"

j2 --format=yaml -o "\${JOB_SCRIPT}" "dishtiny/slurm/competition/straincompetitioncronjob.slurm.sh.jinja" << J2_HEREDOC_EOF
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
endeavor: "\${ENDEAVOR}"
stint: "${STINT}"
series: ${SERIES}
J2_HEREDOC_EOF

chmod +x "\${JOB_SCRIPT}"

################################################################################
echo
echo "Submit Runscript"
echo "----------------"
################################################################################

for retry in {0..20}; do

  if ((\${retry}==20)); then echo "  job submit fail" && exit 123123; fi

  sshpass -p "\${HOST_PASSWORD}" \
    scp -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "\${JOB_SCRIPT}" "\${HOST_USERNAME}@\$(hostname):\${JOB_SCRIPT}" \
  && echo "  job script copy success" \
  || (echo "retrying job script copy (\${retry})" && sleep \$((RANDOM % 10)) && continue)

  echo "sbatch \"\${JOB_SCRIPT}\"" \
  | sshpass -p "\${HOST_PASSWORD}" \
    ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
      "\${HOST_USERNAME}@\$(hostname)" -X \
      'bash -login'  \
  && echo "  job submit success" && break \
  || (echo "retrying job submit (\${retry})" && sleep \$((RANDOM % 10)) && continue)

done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
