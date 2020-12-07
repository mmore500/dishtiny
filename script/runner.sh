#!/bin/bash

################################################################################
echo
echo "Assert Prequisites"
echo "--------------------------------------"
################################################################################

command -v osf >/dev/null || ( echo "no osf client" && exit 1 )
test $OSF_USERNAME || ( echo "no OSF_USERNAME env var" && exit 1 )
test $OSF_PASSWORD || ( echo "no OSF_PASSWORD env var" && exit 1 )
test $PUSHOVER_USER_TOKEN || ( echo "no PUSHOVER_USER_TOKEN env var" && exit 1 )
test $PUSHOVER_APP_TOKEN || ( echo "no PUSHOVER_APP_TOKEN env var" && exit 1 )

################################################################################
echo
echo "Setup Temporary Files"
echo "--------------------------------------"
################################################################################

stdin=$(mktemp)
log=$(mktemp)
context=$(mktemp)
rerun=$(mktemp)
output=$(mktemp)
manifest=$(mktemp)
runner=$(readlink -f $0) # get path to this script

################################################################################
echo
echo "Redirect stdout and stderr to Log File"
echo "--------------------------------------"
################################################################################
exec > >(tee "${log}") 2>&1

################################################################################
echo
echo "Log Job Info"
echo "--------------------------------------"
################################################################################
date
hostname
command -v qstat >/dev/null && qstat -f ${SLURM_JOB_ID}

################################################################################
echo
echo "Setup Exit and Error Traps"
echo "--------------------------------------"
################################################################################

function on_exit {

  echo
  echo "Run Exit Trap"
  echo "-------------"

  echo "log job duration..."
  echo "  date $(date)"
  echo "  SECONDS ${SECONDS}"

  echo "uploading runner"
  osf -p dnh2v upload \
    "${runner}" \
    "data_edges/${DATA_EDGE_ID}/a=runner+action=${DATA_EDGE_ID}+ext=.sh"

  echo "uploading stdin"
  osf -p dnh2v upload \
    "${stdin}" \
    "data_edges/${DATA_EDGE_ID}/a=stdin+action=${DATA_EDGE_ID}+ext=.txt"

  echo "uploading output"
  mkdir -p output
  tar -czf $output output
  osf -p dnh2v upload \
    "${output}" \
    "data_edges/${DATA_EDGE_ID}/a=output+action=${DATA_EDGE_ID}+ext=.tar.gz"

  raw_output_url=$(osf -p dnh2v geturl \
    "data_edges/${DATA_EDGE_ID}/a=output+action=${DATA_EDGE_ID}+ext=.tar.gz" \
  )
  output_url=$(curl -Ls -o /dev/null -w %{url_effective} $raw_output_url)
  echo "output uploaded to ${output_url}"
  echo "  download a copy: curl -L ${output_url}download --output ${DATA_EDGE_ID}.tar.gz"

  echo "uploading output manifest"
  ls output > $manifest
  osf -p dnh2v upload \
    "${manifest}" \
    "data_edges/${DATA_EDGE_ID}/a=manifest+action=${DATA_EDGE_ID}+ext=.txt"

  echo "making rerun script"
  INPUT="$(cat $stdin)"
  echo "#!/bin/bash" > "${rerun}"
  echo "rm -rf ${DATA_EDGE_ID} && mkdir ${DATA_EDGE_ID} && cd ${DATA_EDGE_ID}" \
    >> "${rerun}"
  echo "git clone https://github.com/mmore500/dishtiny" >> "${rerun}"
  echo 'echo $(cd dishtiny && git checkout -q' "${repo_sha})" >> "${rerun}"
  echo "singularity shell docker://mmore500/dishtiny@sha256:${dockerfile_sha} \
<< END_OF_HEREDOC
${INPUT}
END_OF_HEREDOC" >> "${rerun}"
  chmod +x "${rerun}"

  echo "uploading rerun script"
  osf -p dnh2v upload \
    "${rerun}" \
    "data_edges/${DATA_EDGE_ID}/a=rerun+action=${DATA_EDGE_ID}+ext=.sh"

  echo "uploading log"
  osf -p dnh2v upload \
    "${log}" \
    "data_edges/${DATA_EDGE_ID}/a=log+action=${DATA_EDGE_ID}+ext=.txt"

  echo "done uploading"

}

function on_error() {

  echo
  echo "Run Error Trap (FAIL)"
  echo "---------------------"

  echo "   EXIT STATUS ${1}"
  echo "   LINE NO ${2}"

  echo "---------------------"
  echo

  # upload log files
  on_exit

  echo "Sending Pushover Notification"
  bash <(curl https://raw.githubusercontent.com/mmore500/pushover.sh/master/pushover.sh) \
   -T "$PUSHOVER_APP_TOKEN" -U "$PUSHOVER_USER_TOKEN" \
   -u $(osf -p dnh2v geturl \
      "data_edges/${DATA_EDGE_ID}/a=log+action=${DATA_EDGE_ID}+ext=.txt" \
    ) \
    "$SLURM_JOB_NAME failed"

  # reset exit trap
  trap '' EXIT

  exit 1

}

trap 'on_error $? $LINENO' ERR
trap on_exit EXIT

################################################################################
echo
echo "Setup Exit and Error Traps"
echo "--------------------------------------"
################################################################################

export DATA_EDGE_ID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 16 | head -n 1)
echo "DATA_EDGE_ID ${DATA_EDGE_ID}"

################################################################################
echo
echo "Setup Work Directory"
echo "--------------------------------------"
################################################################################
rm -rf $DATA_EDGE_ID && mkdir $DATA_EDGE_ID && cd $DATA_EDGE_ID
pwd

################################################################################
echo
echo "Get Assets"
echo "--------------------------------------"
################################################################################

# setup latest project source
test $checkout_branch || checkout_branch=master
git clone https://github.com/mmore500/dishtiny --recursive -b $checkout_branch
repo_sha=$(cd dishtiny && git rev-parse HEAD)
echo "repo_sha ${repo_sha}"

# get sha of latest docker container
dockerfile_sha=$(\
  singularity exec "docker://mmore500/dishtiny" \
    bash -c 'echo ${SINGULARITY_NAME}' \
)
echo "dockerfile_sha ${dockerfile_sha}"

################################################################################
echo
echo "Do Work"
echo "--------------------------------------"
################################################################################

# pipe input into the container, record a copy
tee "${stdin}" | \
  singularity shell "docker://mmore500/dishtiny@sha256:${dockerfile_sha}"
