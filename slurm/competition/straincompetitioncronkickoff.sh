#!/bin/bash

################################################################################
echo
echo "running straincompetitioncronkickoff.sh"
echo "---------------------------------------"
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
echo "running straincompetitioncronkickoff.sh"
echo "---------------------------------------"
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
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
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

# adapted from https://superuser.com/a/689340
# and https://stackoverflow.com/a/4642975
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/script/host_sbatch.sh" | bash -s "\${JOB_SCRIPT}"

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
