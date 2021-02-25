#!/bin/bash

################################################################################
echo
echo "running robustness100competitionkickoff.sh"
echo "-----------------------------------"
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
echo "running robustness100competitionkickoff.sh"
echo "-----------------------------------"
################################################################################

echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "STINT ${STINT}"
echo "SERIES ${SERIES}"

source ~/.secrets.sh || :

FIRST_SERIES=${SERIES%% *}
ENDEAVOR="\$(( FIRST_SERIES / 1000 ))"
NUM_SERIES="$( echo ${SERIES} | wc -w )"

echo "ENDEAVOR \${ENDEAVOR}"
echo "NUM_SERIES \${NUM_SERIES}"

################################################################################
echo
echo "Generate Tournament Runscripts"
echo "------------------------------"
################################################################################

for JUST_ONE_SERIES in ${SERIES}; do
  for REPLICATE in {0..19}; do

    FIRST_COMPETITOR="s3://dnh2v/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
    SECOND_COMPETITOR="s3://dnh2v/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"

    echo "FIRST_COMPETITOR \${FIRST_COMPETITOR}"
    echo "SECOND_COMPETITOR \${SECOND_COMPETITOR}"

    j2 --format=yaml -o "a=competition+series=\${JUST_ONE_SERIES}+stint=${STINT}+replicate=\${REPLICATE}+ext=.slurm.sh" "dishtiny/slurm/competition/competitionjob.slurm.sh.jinja" << J2_HEREDOC_EOF
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "\${FIRST_COMPETITOR}"
second_competitor_url: "\${SECOND_COMPETITOR}"
second_competitor_attr: "mutate_on_load=100+"
output_url: "s3://dnh2v/endeavor=\${ENDEAVOR}/robustness100-competitions/stage=1+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/"
replicate: "\${REPLICATE}"
endeavor: "\${ENDEAVOR}"
stint: "${STINT}"
series: "\${JUST_ONE_SERIES}"
J2_HEREDOC_EOF

  done
done

################################################################################
echo
echo "Bundle and Submit Generated Runscripts"
echo "--------------------------------------"
################################################################################

echo "num generated runscripts \$(ls *.slurm.sh | wc -l)"

# uses slurm stoker script, which zips all runscripts in the current directory
# inside itself, then submits itself as a job to gradually feed runscripts onto
# the queue

dishtiny/script/slurm_stoker_containerized_kickoff.sh "${CONTAINER_TAG}" "${REPO_SHA}"

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
