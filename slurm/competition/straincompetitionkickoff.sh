#!/bin/bash

################################################################################
echo
echo "running straincompetitionkickoff.sh"
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
echo "running straincompetitionkickoff.sh"
echo "-----------------------------------"
################################################################################

echo "CONTAINER_TAG ${CONTAINER_TAG}"
echo "REPO_SHA ${REPO_SHA}"
echo "STINT ${STINT}"
echo "SERIES ${SERIES}"

source ~/.secrets.sh || :

ENDEAVOR="$(( STINT / 1000 ))"
TOURNAMENT_LAYOUT_SEED="${STINT}"
NUM_SERIES="$( echo ${SERIES} | wc -w )"
NUM_COMPETITIONS_EACH="\$(( NUM_SERIES - 1 > 20 ? 20 : NUM_SERIES - 1 ))"

echo "ENDEAVOR \${ENDEAVOR}"
echo "TOURNAMENT_LAYOUT_SEED \${TOURNAMENT_LAYOUT_SEED}"
echo "NUM_SERIES \${NUM_SERIES}"
echo "NUM_COMPETITIONS_EACH \${NUM_COMPETITIONS_EACH}"

################################################################################
echo
echo "Generate Tournament Runscripts"
echo "------------------------------"
################################################################################

python3 "dishtiny/script/layout_interstrain_tournament.py" "\${TOURNAMENT_LAYOUT_SEED}" "\${NUM_COMPETITIONS_EACH}" ${SERIES} | while read LINE ; do

  read -r FIRST_COMPETITOR_SERIES SECOND_COMPETITOR_SERIES <<<"\${LINE}"

  FIRST_COMPETITOR="s3://dnh2v/endeavor=\${ENDEAVOR}/genomes/stint=${STINT}/series=\${FIRST_COMPETITOR_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${FIRST_COMPETITOR_SERIES}+stint=${STINT}+thread=0+ext=.json.gz"
  SECOND_COMPETITOR="s3://dnh2v/endeavor=\${ENDEAVOR}/genomes/stint=${STINT}/series=\${SECOND_COMPETITOR_SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${SECOND_COMPETITOR_SERIES}+stint=${STINT}+thread=0+ext=.json.gz"

  echo "FIRST_COMPETITOR \${FIRST_COMPETITOR}"
  echo "SECOND_COMPETITOR \${SECOND_COMPETITOR}"

  j2 --format=yaml -o "a=competition+first_competitor=\${FIRST_COMPETITOR_SERIES}+second_competitor=\${SECOND_COMPETITOR_SERIES}+stint=${STINT}+ext=.slurm.sh" "dishtiny/slurm/competition/competitionjob.slurm.sh.jinja" << J2_HEREDOC_EOF
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "\${FIRST_COMPETITOR}"
second_competitor_url: "\${SECOND_COMPETITOR}"
output_url: "s3://dnh2v/endeavor=\${ENDEAVOR}/strain-competitions/stint=${STINT}/"
replicate: 0
endeavor: "\${ENDEAVOR}"
stint: "${STINT}"
J2_HEREDOC_EOF

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

echo dishtiny/script/slurm_stoker_containerized_kickoff.sh "${CONTAINER_TAG}" "${REPO_SHA}"

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
