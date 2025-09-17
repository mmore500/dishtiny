#!/bin/bash

################################################################################
echo
echo "running restint.sh"
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

if (( "$#" < 6 )); then
  echo "USAGE: [read_bucket] [write_bucket] [configpack] [container_tag] [repo_sha] [stint] [series...]"
  exit 1
fi

READ_BUCKET="${1}"
echo "READ_BUCKET ${READ_BUCKET}"
shift

WRITE_BUCKET="${1}"
echo "WRITE_BUCKET ${WRITE_BUCKET}"
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
  -p "${READ_BUCKET}" -u mmore500 -s dishtiny \
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
echo "running restint.sh"
echo "------------------------"
################################################################################

echo "READ_BUCKET ${READ_BUCKET}"
echo "WRITE_BUCKET ${WRITE_BUCKET}"
echo "CONFIGPACK ${CONFIGPACK}"
echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "STINT ${STINT}"
echo "SERIES ${SERIES}"

source ~/.secrets.sh || :

TEMPLATE_PATH="\$(mktemp)"
echo "TEMPLATE_PATH \${TEMPLATE_PATH}"
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/slurm/restint/restintjob.slurm.sh.jinja" > "\${TEMPLATE_PATH}"
echo "template downloaded"

for just_one_series in ${SERIES}; do

  JOB_SCRIPT="\$(mktemp)"

  echo "series \${just_one_series}"
  echo "JOB_SCRIPT \${JOB_SCRIPT}"

  echo "running j2"
  echo "which j2: \$(which j2)"

  j2 --format=yaml -o "\${JOB_SCRIPT}" "\${TEMPLATE_PATH}" << J2_HEREDOC_EOF
read_bucket: "${READ_BUCKET}"
write_bucket: "${WRITE_BUCKET}"
configpack: "${CONFIGPACK}"
container_tag: "${CONTAINER_TAG}"
repo_sha: "${REPO_SHA}"
series: "\${just_one_series}"
stint: "${STINT}"
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
