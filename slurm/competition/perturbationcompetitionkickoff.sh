#!/bin/bash

################################################################################
echo
echo "running perturbationcompetitionkickoff.sh"
echo "-------------------------------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" < 10 )); then
  echo "USAGE: [bucket] [configpack] [container_tag] [repo_sha] [extrospective/introspective/writable] [exchange/rotate] [intermittent_p] [target_idx] [stint] [series...]"
  echo "where [target_idx] can be empty string \"\" or a brace expandable"
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

STATE_TARGET="${1}"
echo "STATE_TARGET ${STATE_TARGET}"
shift

PERTURBATION="${1}"
echo "PERTURBATION ${PERTURBATION}"
shift

INTERMITTENT_P="${1}"
echo "INTERMITTENT_P ${INTERMITTENT_P}"
shift

TARGET_IDX="${1}"
echo "TARGET_IDX ${TARGET_IDX}"
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
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),\$0 }' L=\$1 \$0
}
trap 'printerr \$LINENO' ERR

################################################################################
echo
echo "running perturbationcompetitionkickoff.sh"
echo "-------------------------------------------------"
################################################################################

echo "BUCKET ${BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "STATE_TARGET ${STATE_TARGET}"
echo "PERTURBATION ${PERTURBATION}"
echo "INTERMITTENT_P ${INTERMITTENT_P}"
echo "TARGET_IDX ${TARGET_IDX}"
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

# "" on end for empty string
for JUST_ONE_TARGET_IDX in $(eval echo ${TARGET_IDX})""; do
  for JUST_ONE_SERIES in ${SERIES}; do
    for REPLICATE in 0; do

      FIRST_COMPETITOR="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
      SECOND_COMPETITOR="\${FIRST_COMPETITOR}"

      echo "FIRST_COMPETITOR \${FIRST_COMPETITOR}"
      echo "SECOND_COMPETITOR \${SECOND_COMPETITOR}"
      echo "SERIES \${JUST_ONE_SERIES}"
      echo "TARGET_IDX \${JUST_ONE_TARGET_IDX}"

      j2 --format=yaml -o "a=competition+series=\${JUST_ONE_SERIES}+stint=${STINT}+target_idx=\${JUST_ONE_TARGET_IDX}+replicate=\${REPLICATE}+ext=.slurm.sh" "dishtiny/slurm/competition/competitionjob.slurm.sh.jinja" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "\${FIRST_COMPETITOR}"
second_competitor_url: "\${SECOND_COMPETITOR}"
second_competitor_attrs: "set_intermittent_${STATE_TARGET}_state_${PERTURBATION}_probability=${INTERMITTENT_P}+set_${STATE_TARGET}_state_target_idx=\${JUST_ONE_TARGET_IDX}+"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/perturbation-${STATE_TARGET}-state-${PERTURBATION}-competitions/stage=1+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/intermittent_p=${INTERMITTENT_P}+target_idx=\${JUST_ONE_TARGET_IDX}/"
replicate: "\${REPLICATE}"
endeavor: "\${ENDEAVOR}"
stint: "${STINT}"
series: "\${JUST_ONE_SERIES}"
J2_HEREDOC_EOF

  done
  done
done

################################################################################
echo
echo "Bundle and Submit Generated Runscripts"
echo "--------------------------------------"
################################################################################

echo "num generated runscripts \$(ls *.slurm.sh | wc -l)"

if test -v SLURM_STOKER_CONSOLIDATION_DIR; then

echo "SLURM_STOKER_CONSOLIDATION_DIR \${SLURM_STOKER_CONSOLIDATION_DIR}"

mkdir -p "\${SLURM_STOKER_CONSOLIDATION_DIR}"
for target in *slurm.sh; do
  cp "\${target}" "\${SLURM_STOKER_CONSOLIDATION_DIR}/\${RANDOM}_\${target}"
done

else

# uses slurm stoker script, which zips all runscripts in the current directory
# inside itself, then submits itself as a job to gradually feed runscripts onto
# the queue

dishtiny/script/slurm_stoker_containerized_kickoff.sh "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "intermittent-${STATE_TARGET}-state-${PERTURBATION}-competition~configpack%${CONFIGPACK}~series%${SERIES%% *}...~stint%${STINT}~intermittent_p%${INTERMITTENT_P}"

fi

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
