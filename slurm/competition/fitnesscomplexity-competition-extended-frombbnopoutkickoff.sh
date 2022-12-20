#!/bin/bash

################################################################################
echo
echo "running fitnesscomplexity-competition-extended-frombbnopoutkickoff.sh"
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
echo "fitnesscomplexity-competition-extended-frombbnopoutkickoff.sh"
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
cd "$(mktemp -d)"
pwd

################################################################################
echo
echo "STINT \${STINT}:"
echo "Download wildtype genome"
echo "----------------------------------------------------------"
################################################################################

WILDTYPE_URI="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=\${STINT}/series=${SERIES}/a=genome+criteria=abundance+morph=wildtype+proc=0+series=${SERIES}+stint=\${STINT}+thread=0+variation=master+ext=.json.gz"
WILDTYPE_GENOME="\$(mktemp -d)/\$(basename \${WILDTYPE_URI})"

echo "downloading \${WILDTYPE_URI} to \${WILDTYPE_GENOME}"
for retry in {1..20}; do
  aws s3 cp --quiet \
    "\${WILDTYPE_URI}" \
    "\${WILDTYPE_GENOME}" \
    && echo "  \${WILDTYPE_URI} download success" && break \
    || (echo "retrying \${WILDTYPE_URI} download (\${retry})" && sleep \$((RANDOM % 10)))
  if ((\${retry}==20)); then echo "\${WILDTYPE_URI} download fail" && exit 123123; fi
done

################################################################################
echo
echo "STINT \${STINT}:"
echo "Download biotic background phenotype_equivalent_nopout genome"
echo "----------------------------------------------------------"
################################################################################

BB_PE_NOPOUT_URI="s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=1+what=generated/stint=\${STINT}/series=${SERIES}/a=genome+bgpop=1+criteria=abundance+morph=jenga_phenotype_equivalent_nopout_with_biotic_background+proc=0+series=${SERIES}+stint=\${STINT}+thread=0+variation=master+ext=.json.gz"
PHENOTYPE_EQUIVALENT_GENOME="\$(mktemp -d)/\$(basename \${BB_PE_NOPOUT_URI})"

echo "downloading \${BB_PE_NOPOUT_URI} to \${PHENOTYPE_EQUIVALENT_GENOME}"
for retry in {1..20}; do
  aws s3 cp --quiet \
    "\${BB_PE_NOPOUT_URI}" \
    "\${PHENOTYPE_EQUIVALENT_GENOME}" \
    && echo "  \${BB_PE_NOPOUT_URI} download success" && break \
    || (echo "retrying \${BB_PE_NOPOUT_URI} download (\${retry})" && sleep \$((RANDOM % 10)))
  if ((\${retry}==20)); then echo "\${BB_PE_NOPOUT_URI} download fail" && exit 123123; fi
done

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

echo "PHENOTYPE_EQUIVALENT_GENOME \${PHENOTYPE_EQUIVALENT_GENOME}"
echo "WILDTYPE_GENOME \${WILDTYPE_GENOME}"

################################################################################
echo
echo "STINT \${STINT}:"
echo "Generate Nopped Op Variants of phenotype_equivalent_nopout"
echo "----------------------------------------------------------"
################################################################################

python3 "\${REPRO_WORK_DIRECTORY}/dishtiny/script/make_nopped_op_variants.py" "\${WILDTYPE_GENOME}" "\${PHENOTYPE_EQUIVALENT_GENOME}"

################################################################################
echo
echo "STINT \${STINT}:"
echo "Upload Nopped Op Variants of phenotype_equivalent_nopout"
echo "--------------------------------------------------------"
################################################################################

shopt -s nullglob

echo "uploading any genome files..."
for p in *a=genome*morph=wildtype*variation=* ; do
  f="\$(basename "\${p}")"
  echo "uploading \${p} -> \${f}"
  for retry in {1..20}; do
    aws s3 cp --quiet \
      "\${p}" \
      "s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes-frombbnopout/stage=2+what=generated/stint=\${STINT}/series=${SERIES}/\${f}" \
    && echo "  \$f upload success" && break \
      || (echo "retrying \${f} upload (\${retry})" && sleep \$((RANDOM % 10)))
    if ((\${retry}==20)); then echo "\$f upload fail" && exit 123123; fi
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

shopt -u nullglob

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
echo "Generate Competition Runscripts"
echo "---------------------------------------------"
################################################################################

shopt -s nullglob

wt=\$(basename "\${WILDTYPE_GENOME}")

for variant in *a=genome*morph=wildtype*variation=* ; do

echo "variant \${variant}"

variation="\$(echo "\${variant}" | keyname extract variation)"
echo "variation \${variation}"

JOB_SCRIPT="\$( keyname pack \
  --a "wt_vs_variant_backgroundbb" --variation "\${variation}" --ext ".slurm.sh" \
)"

j2 --format=yaml -o "\${JOB_SCRIPT}" "\${JOB_TEMPLATE}" << J2_HEREDOC_EOF
bucket: ${BUCKET}
configpack: ${CONFIGPACK}
container_tag: ${CONTAINER_TAG}
repo_sha: ${REPO_SHA}
first_competitor_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes/stage=0+what=generated/stint=\${STINT}/series=${SERIES}/\${wt}"
second_competitor_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/genomes-frombbnopout/stage=2+what=generated/stint=\${STINT}/series=${SERIES}/\${variant}"
output_url: "s3://${BUCKET}/endeavor=\${ENDEAVOR}/variant-competitions-extended-frombbnopout/stage=3+what=generated/stint=\${STINT}/series=${SERIES}/"
replicate: 0
endeavor: \${ENDEAVOR}
stint: \${STINT}
series: ${SERIES}
run_seconds: 3600
J2_HEREDOC_EOF

chmod +x "\${JOB_SCRIPT}"

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
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- https://raw.githubusercontent.com/mmore500/dishtiny/${REPO_SHA}/script/slurm_stoker_containerized_kickoff.sh | bash -s "${BUCKET}" "${CONTAINER_TAG}" "${REPO_SHA}" "variant-competition~configpack%${CONFIGPACK}~series%${SERIES}~stint%\${STINT}"

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
