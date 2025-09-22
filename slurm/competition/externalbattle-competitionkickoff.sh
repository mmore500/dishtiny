#!/bin/bash

################################################################################
echo
echo "running externalcompetition-competitionkickoff.sh"
echo "-------------------------------------------------------------------------"
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
  echo "USAGE: [bucket] [configpack] [container_tag] [repo_sha] [series] [stints...]"
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

SERIES="${1}"
echo "SERIES ${SERIES}"
shift

STINTS="${@}"
echo "STINTS ${STINTS}"


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
echo "STINT \${STINT}:"
echo "running externalcompetition-competitionkickoff.sh"
echo "-------------------------------------------------------------------------"
################################################################################

echo "BUCKET ${BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "SERIES ${SERIES}"
echo "STINTS ${STINTS}"

source ~/.secrets.sh || :

ENDEAVOR="$(( SERIES / 1000 ))"
NUM_STINTS="$( echo ${STINTS} | wc -w )"

echo "ENDEAVOR \${ENDEAVOR}"
echo "NUM_SERIES \${NUM_SERIES}"

for STINT in ${STINTS}; do

echo
echo "STINT \${STINT}:"
echo "Dispatching Fitness Complexity Competitions"
echo "==========================================="

(
cd "\$(mktemp -d)"
pwd

################################################################################
echo
echo "STINT \${STINT}:"
echo "Get Runscript Template"
echo "----------------------------------------"
################################################################################

# run jinja on template
JOB_TEMPLATE="\$(mktemp)"
echo "JOB_TEMPLATE \${JOB_TEMPLATE}"

for retry in {1..20}; do
  curl \
    -o "\${JOB_TEMPLATE}" \
    "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/slurm/competition/competitionjob.slurm.sh.jinja" \
  && echo "  job template curl success" && break \
  || (echo "retrying job template curl (\${retry})" && sleep \$((RANDOM % 10)))
  if ((\${retry}==20)); then echo "job template curl fail" && exit 123123; fi
done

################################################################################
echo
echo "STINT \${STINT}:"
echo "Generate External Competition Runscripts"
echo "-------------------------------------------------------------------------"
################################################################################

shopt -s nullglob

counter=0
for comparator_series in {16000..16039}; do
for replicate in 0 1; do

echo "counter \${counter}"
echo "replicate \${replicate}"

JOB_SCRIPT="\$( keyname pack \
  --a "externalbattle" --comparator_series "\${comparator_series}" --replicate "\${replicate}" --counter "\${counter}" --ext ".slurm.sh" \
)"

j2 --format=yaml -o "\${JOB_SCRIPT}" "\${JOB_TEMPLATE}" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/populations/stage=0+what=generated/stint=\${STINT}/series=${SERIES}/a=population+proc=0+series=${SERIES}+stint=\${STINT}+thread=0+variation=master+ext=.bin.xz"
second_competitor_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/populations/stage=0+what=generated/stint=\${STINT}/series=\${comparator_series}/a=population+proc=0+series=\${comparator_series}+stint=\${STINT}+thread=0+variation=master+ext=.bin.xz"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/external-battles/stage=1+what=generated/stint=\${STINT}/series=${SERIES}/"
replicate: \${replicate}
endeavor: \${ENDEAVOR}
stint: \${STINT}
series: ${SERIES}
run_seconds: 600
J2_HEREDOC_EOF

chmod +x "\${JOB_SCRIPT}"
counter=\$((counter+1))

done
done

shopt -u nullglob

################################################################################
echo
echo "STINT \${STINT}:"
echo "Bundle and Submit Generated Runscripts"
echo "--------------------------------------"
################################################################################

echo "num generated runscripts \$(ls *.slurm.sh | wc -l)"

# uses slurm stoker script, which zips all runscripts in the current directory
# inside itself, then submits itself as a job to gradually feed runscripts onto
# the queue

# if there are any .slurm.sh files
if ls *.slurm.sh 1> /dev/null 2>&1; then

# adapted from https://superuser.com/a/689340
# and https://stackoverflow.com/a/4642975
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/script/slurm_stoker_containerized_kickoff.sh | bash -s "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "control-competition~configpack%${CONFIGPACK}~series%${SERIES}~stint%\${STINT}"

else

echo "WARNING no variant competition runscripts generated"
echo "skipping slurm_stoker kickoff"
ls

fi

)
done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
