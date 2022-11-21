#!/bin/bash

################################################################################
echo
echo "running nopout-bioticbackgroundkickoff.sh"
echo "------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

if (( "$#" < 7 )); then
  echo "USAGE: [bucket] [configpack] [container_tag] [repo_sha] [followup_freq] [stint] [series...]"
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

FOLLOWUP_FREQ="${1}"
echo "FOLLOWUP_FREQ ${FOLLOWUP_FREQ}"
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
echo "running nopout-bioticbackgroundkickoff.sh"
echo "------------------------"
################################################################################

echo "BUCKET ${BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "FOLLOWUP_FREQ ${FOLLOWUP_FREQ}"
echo "STINT ${STINT}"
echo "SERIES ${SERIES}"

source ~/.secrets.sh || :

for just_one_series in ${SERIES}; do

  JOB_SCRIPT="\$(mktemp)"

  echo "series \${just_one_series}"
  echo "JOB_SCRIPT \${JOB_SCRIPT}"

  j2 --format=yaml -o "\${JOB_SCRIPT}" "dishtiny/slurm/genomes/nopout-bioticbackgroundjob.slurm.sh.jinja" << J2_HEREDOC_EOF
bucket: "${BUCKET}"
configpack: "${CONFIGPACK}"
container_tag: "${CONTAINER_TAG}"
repo_sha: "${REPO_SHA}"
series: "\${just_one_series}"
stint: "${STINT}"
target_genome_url: "s3://${BUCKET}/endeavor=\$(( just_one_series / 1000 ))/genomes/stage=0+what=generated/stint=${STINT}/series=\${just_one_series}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${just_one_series}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
background_population_attrs: "filter_except_lowest_root+"
background_population_url: "s3://${BUCKET}/endeavor=\$(( just_one_series / 1000 ))/evolve/populations/stage=0+what=generated/stint=${STINT}/series=\${just_one_series}/a=population+proc=0+series=\${just_one_series}+stint=${STINT}+thread=0+variation=master+ext=.bin.xz"
followup_freq: "${FOLLOWUP_FREQ}"
J2_HEREDOC_EOF

  # adapted from https://superuser.com/a/689340
  # and https://stackoverflow.com/a/4642975
  wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/script/host_sbatch.sh" | bash -s "\${JOB_SCRIPT}"


done

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
