#!/bin/bash

################################################################################
echo
echo "running noncritical-phenotypeequivalent-nopinterpolation-competitionkickoff.sh"
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
pwd

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
echo "running noncritical-phenotypeequivalent-nopinterpolation-competitionkickoff.sh"
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
TOURNAMENT_LAYOUT_SEED="${STINT}"
NUM_SERIES="$( echo ${SERIES} | wc -w )"
NUM_COMPETITIONS_EACH="\$(( NUM_SERIES - 1 > 20 ? 20 : NUM_SERIES - 1 ))"

echo "ENDEAVOR \${ENDEAVOR}"
echo "TOURNAMENT_LAYOUT_SEED \${TOURNAMENT_LAYOUT_SEED}"
echo "NUM_SERIES \${NUM_SERIES}"
echo "NUM_COMPETITIONS_EACH \${NUM_COMPETITIONS_EACH}"

################################################################################
echo
echo "Generate Nop-Interpolated Variants"
echo "------------------------------"
################################################################################

SOURCE_GENOME_DIR="\$(mktemp -d)"

for JUST_ONE_SERIES in ${SERIES}; do

  MORE_OPS_URL="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=1+what=generated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=phenotype_equivalent_nopout+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
  LESS_OPS_URL="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=3+what=tabulated/stint=${STINT}/series=\${JUST_ONE_SERIES}/a=genome+criteria=abundance+morph=noncritical_nopout+proc=0+series=\${JUST_ONE_SERIES}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"

  MORE_OPS_PATH="\${SOURCE_GENOME_DIR}/\$(basename \${MORE_OPS_URL})"
  LESS_OPS_PATH="\${SOURCE_GENOME_DIR}/\$(basename \${LESS_OPS_URL})"

  echo "downloading \${MORE_OPS}"
  for retry in {1..20}; do
    aws s3 cp --quiet \
      "\${MORE_OPS_URL}" \
      "\${MORE_OPS_PATH}" \
      && echo "  \${MORE_OPS_URL} download success" && break \
      || (echo "retrying \${MORE_OPS_URL} download (\${retry})" && sleep \$((RANDOM % 10)))
    if ((\${retry}==20)); then echo "\${MORE_OPS_URL} download fail" && exit 123123; fi
  done

  echo "downloading \${LESS_OPS}"
  for retry in {1..20}; do
    aws s3 cp --quiet \
      "\${LESS_OPS_URL}" \
      "\${LESS_OPS_PATH}" \
      && echo "  \${LESS_OPS_URL} download success" && break \
      || (echo "retrying \${LESS_OPS_URL} download (\${retry})" && sleep \$((RANDOM % 10)))
    if ((\${retry}==20)); then echo "\${LESS_OPS_URL} download fail" && exit 123123; fi
  done

  python3 \${REPRO_WORK_DIRECTORY}/dishtiny/script/make_nop_interpolated_variants.py "\${LESS_OPS_PATH}" "\${MORE_OPS_PATH}" 50

done

################################################################################
echo
echo "Upload Nop-Interpolated Variants"
echo "--------------------------------"
################################################################################

for p in *a=genome*+ext=.json.gz; do
  f="\$(basename "\${p}")"
  echo "uploading \${p} -> \${f}"
  cur_series="\$(echo \${f} | keyname extract series)"
  echo "extracted series \${series}"
  for retry in {1..20}; do
    aws s3 cp --quiet \
      "\${p}" \
      "s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=4+what=tabulated/stint=${STINT}/series=\${cur_series}/" \
    && echo "  \${f} upload success" && break \
    || (echo "retrying \${f} upload (\${retry})" && sleep \$((RANDOM % 10)))
    if ((\${retry}==20)); then echo "\${f} upload fail" && exit 123123; fi
  done & pids+=(\$!)

  # limit to twenty concurrent upload jobs
  while (( \$(jobs -p | wc -l) > 20 )); do sleep 1; done

done

# wait on all forked upload jobs
for pid in "\${pids[@]}"; do
  # if child process fails, we fail
  wait "\${pid}"
done

unset pids

################################################################################
echo
echo "Generate Interpolation Competition Runscripts"
echo "---------------------------------------------"
################################################################################

# each interpolated genome needs to be competed against
# phenotype-equivalent nopout
# (could be against wildtype, but this is the more conservative choice)

for cur_genome in *a=genome*+ext=.json.gz; do

  cur_series="\$(echo \${cur_genome} | keyname extract series)"
  cur_interpolation_step="\$(echo \${cur_genome} | keyname extract nop_interpolation_step)"

  FIRST_COMPETITOR="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=${STINT}/series=\${cur_series}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=\${cur_series}+stint=${STINT}+thread=0+variation=master+ext=.json.gz"
  SECOND_COMPETITOR="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=4+what=tabulated/stint=${STINT}/series=\${cur_series}/\${cur_genome}"

  echo "FIRST_COMPETITOR \${FIRST_COMPETITOR}"
  echo "SECOND_COMPETITOR \${SECOND_COMPETITOR}"

  j2 --format=yaml -o "a=competition+nop_interpolation_step=\${cur_interpolation_step}+series=\${cur_series}+stint=${STINT}+ext=.slurm.sh" "\${REPRO_WORK_DIRECTORY}/dishtiny/slurm/competition/competitionjob.slurm.sh.jinja" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "\${FIRST_COMPETITOR}"
second_competitor_url: "\${SECOND_COMPETITOR}"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/noncritical-phenotypeneutral-nopinterpolation-competitions/stage=5+what=generated/stint=${STINT}/"
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

\${REPRO_WORK_DIRECTORY}/dishtiny/script/slurm_stoker_containerized_kickoff.sh "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "noncritical-phenotypeequivalent-nopinterpolation-competition~configpack%${CONFIGPACK}~series%${SERIES%% *}...~stint%${STINT}"

fi

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

REPRO_RUNNER_HEREDOC_EOF
