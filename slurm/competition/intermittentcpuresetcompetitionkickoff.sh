#!/bin/bash

################################################################################
echo
echo "running intermittentcpuresetcompetitionkickoff.sh"
echo "-------------------------------------------------"
################################################################################

# fail on error
set -e

if (( "$#" < 6 )); then
  echo "USAGE: [bucket] [configpack] [container_tag] [repo_sha] [intermittent_p] [stint] [series...]"
  exit 1
fi

BUCKET="${1}"
echo "BUCKET ${BUCKET}"
shift

CONFIGPACK="${1}"
echo "CONFIGPACK ${CONFIGPACK}"
shift

CONTAINER_TAG="${1}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
shift

REPO_SHA="${1}"
echo "REPO_SHA ${REPO_SHA}"
shift

INTERMITTENT_P="${1}"
echo "INTERMITTENT_P ${INTERMITTENT_P}"
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
  -p "${BUCKET}" -u mmore500 -s dishtiny \
  --repo_sha "${REPO_SHA}" --container_tag "${CONTAINER_TAG}" \
  << REPRO_RUNNER_HEREDOC_EOF

# fail on error
set -e

################################################################################
echo
echo "running intermittentcpuresetcompetitionkickoff.sh"
echo "-------------------------------------------------"
################################################################################

echo "BUCKET ${BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "INTERMITTENT_P ${INTERMITTENT_P}"
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
  for REPLICATE in 0; do

    FIRST_COMPETITOR="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
    SECOND_COMPETITOR="\${FIRST_COMPETITOR}"

    echo "FIRST_COMPETITOR \${FIRST_COMPETITOR}"
    echo "SECOND_COMPETITOR \${SECOND_COMPETITOR}"

    j2 --format=yaml -o "a=competition+series=\${JUST_ONE_SERIES}+stint=${STINT}+replicate=\${REPLICATE}+ext=.slurm.sh" "dishtiny/slurm/competition/competitionjob.slurm.sh.jinja" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "\${FIRST_COMPETITOR}"
second_competitor_url: "\${SECOND_COMPETITOR}"
second_competitor_attrs: "set_intermittent_cpu_reset_probability=${INTERMITTENT_P}+"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/intermittent-cpu-reset-competitions/p=${INTERMITTENT_P}/stage=1+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/"
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

dishtiny/script/slurm_stoker_containerized_kickoff.sh "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "intermittent-cpu-reset-competition~configpack%${CONFIGPACK}~series%${SERIES%% *}...~stint%${STINT}~intermittent_p%${INTERMITTENT_P}"

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
