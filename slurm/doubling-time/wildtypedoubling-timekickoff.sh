#!/bin/bash

################################################################################
echo
echo "running wildtypedoubling-timekickoff.sh"
echo "-----------------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" < 6 )); then
  echo "USAGE: [bucket] [configpack] [container_tag] [repo_sha] [stint] [series...]"
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
echo "running wildtypedoubling-timekickoff.sh"
echo "-----------------------------------"
################################################################################

echo "BUCKET ${BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
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

  SUBJECT="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"

  echo "SUBJECT \${SUBJECT}"

  j2 --format=yaml -o "a=doubling-time+series=\${JUST_ONE_SERIES}+stint=${STINT}+replicate=\${REPLICATE}+ext=.slurm.sh" "dishtiny/slurm/doubling-time/doubling-timejob.slurm.sh.jinja" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
subject_url: "\${SUBJECT}"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/wildtype-doubling-time/stage=1+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/"
endeavor: "\${ENDEAVOR}"
stint: "${STINT}"
series: "\${JUST_ONE_SERIES}"
J2_HEREDOC_EOF

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

dishtiny/script/slurm_stoker_containerized_kickoff.sh "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "wildtype-doubling-time~configpack%${CONFIGPACK}~series%${SERIES%% *}...~stint%${STINT}"

fi

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
