#!/bin/bash

set -e

usage="$(basename "$0"): run reproducible, self-documenting computations

./$(basename "$0")
  to enter an interactive shell session, then <ctrl-d> to complete

cat script.sh | ./$(basename "$0")
  to run a bash script as a session, automatically completes after script executes

for each session:

  1. a session ID (\"REPRO_ID\") is generated

  2. a work directory is created within the current working directory

  3. a folder called output is created within the work directory

  4. a shallow, recursive clone of the project's GitHub repository is performed in the work directory
    * the repository's sha is recorded for reproducibility

  5. the project's Docker container is fetched
    * Docker performs caching to save bandwidth for repeated accesses
    * the container's sha is recorded for reproducibility

  6. a shell session is launched on the Docker container using Singularity

  7. during the shell session, all commands are recorded

  8. shell session completes
    * if any files from the session should be saved, be sure to put them in the output folder during the shell session
    * bonus points if you use the REPRO_ID environment variable in output filenames so that the files can be traced back to this session

  9. the following artifacts are added to the OSF project's storage
    * \"repro/\${REPRO_ID}/a=runner+repro=\${REPRO_ID}+ext=.sh\"
      * this script
    * \"repro/\${REPRO_ID}/a=stdin+repro=\${REPRO_ID}+ext=.txt\"
      * commands recorded during the shell session
    * \"repro/\${REPRO_ID}/a=output+repro=\${REPRO_ID}+ext=.tar.gz\"
      * tar.gz archive of the output folder
    * \"repro/\${REPRO_ID}/a=manifest+repro=\${REPRO_ID}+ext=.tar.gz\"
      * hierarchical listing of output folder contents
    * \"repro/\${REPRO_ID}/a=rerun+repro=\${REPRO_ID}+ext=.sh\"
      * script to rerun computations from the current session
      * using sha's to ensure the same exact same source and container
    * \"repro/\${REPRO_ID}/a=log+repro=\${REPRO_ID}+ext=.txt\"
      * output from the current session

  10. if the shell session exited an error code, a pushover notification is dispatched
    * contains \$SLURM_JOB_NAME
    * contains hyperlink to the job log file on OSF

required environment variables:
  OSF_USERNAME
  OSF_PASSWORD
  PUSHOVER_USER_TOKEN
  PUSHOVER_APP_TOKEN

required arguments:
  -p / --project  osf project ID
  -s / --slug  slug used to identify project on dockerhub and github
  -u / --username  username used for dockerhub and github

optional arguments:
  -b / --branch  source branch to check out (default: master)
  -c / --container_sha sha of docker container to run in (default: none)
  -h / --help  show this message and exit
  -r / --repo_sha repo sha to check out (default: none)

exported environment variables:
  REPRO_ID a 16-character string uniquely identifying this session
"

################################################################################
echo
echo "Parse Arguments"
echo "--------------------------------------"
################################################################################

# adapted from https://stackoverflow.com/a/33826763
while [[ "$#" -gt 0 ]]; do
  case $1 in
    -b|--branch) arg_branch="$2"; shift ;; # source branch to check out
    -c|--container_sha) container_sha="$2"; shift ;; # container sha to run in
    -h|--help) echo "$usage"; exit;; # print help message
    -p|--project) arg_project="$2"; shift ;; # osf project id
    -r|--repo_sha) repo_sha="$2"; shift ;; # source sha to check out
    -s|--slug) arg_slug="$2"; shift ;; # github/dockerhub project slug
    -u|--username) arg_username="$2"; shift ;; # github/dockerhub username
    *) echo "Unknown argument passed: $1"; exit 1 ;;
  esac
  shift
done

# default arguments
test $arg_branch || arg_branch=master

################################################################################
echo
echo "Assert Prequisites"
echo "--------------------------------------"
################################################################################

command -v osf >/dev/null \
  || python3 -m pip install --user git+https://github.com/mmore500/osfclient.git \
  || ( echo "no osf client" && exit 1 )
test $OSF_USERNAME || ( echo "no OSF_USERNAME env var" && exit 1 )
test $OSF_PASSWORD || ( echo "no OSF_PASSWORD env var" && exit 1 )
test $PUSHOVER_USER_TOKEN || ( echo "no PUSHOVER_USER_TOKEN env var" && exit 1 )
test $PUSHOVER_APP_TOKEN || ( echo "no PUSHOVER_APP_TOKEN env var" && exit 1 )
test $arg_project || ( echo "no --project arg" && exit 1 )
test $arg_slug || ( echo "no --slug arg" && exit 1 )
test $arg_username || ( echo "no --username arg" && exit 1 )

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
echo "arg_branch $arg_branch"
echo "arg_project $arg_project"
echo "arg_slug $arg_slug"
echo "arg_username $arg_username"

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
  osf -p $arg_project upload \
    "${runner}" \
    "repro/${REPRO_ID}/a=runner+repro=${REPRO_ID}+ext=.sh"

  echo "uploading stdin"
  osf -p $arg_project upload \
    "${stdin}" \
    "repro/${REPRO_ID}/a=stdin+repro=${REPRO_ID}+ext=.txt"

  echo "uploading output"
  tar -czf $output output
  osf -p $arg_project upload \
    "${output}" \
    "repro/${REPRO_ID}/a=output+repro=${REPRO_ID}+ext=.tar.gz"

  raw_output_url=$(osf -p $arg_project geturl \
    "repro/${REPRO_ID}/a=output+repro=${REPRO_ID}+ext=.tar.gz" \
  )
  output_url=$(curl -Ls -o /dev/null -w %{url_effective} $raw_output_url)
  echo "output uploaded to ${output_url}"
  echo "  download a copy: curl -L ${output_url}download --output ${REPRO_ID}.tar.gz"

  echo "uploading output manifest"
  ls output > $manifest
  osf -p $arg_project upload \
    "${manifest}" \
    "repro/${REPRO_ID}/a=manifest+repro=${REPRO_ID}+ext=.txt"

  echo "making rerun script"
  INPUT="$(cat $stdin)"
  echo "#!/bin/bash" > "${rerun}"
  echo "rm -rf ${REPRO_ID} && mkdir ${REPRO_ID} && cd ${REPRO_ID}" \
    >> "${rerun}"
  echo "mkdir ${arg_slug}" >> "${rerun}"
  echo "git -C ${arg_slug} init" >> "${rerun}"
  echo "git -C ${arg_slug} remote add origin https://github.com/${arg_username}/${arg_slug}.git" >> "${rerun}"
  echo "git -C ${arg_slug} fetch --depth 1 origin ${repo_sha}" >> "${rerun}"
  echo "git -C ${arg_slug} checkout FETCH_HEAD" >> "${rerun}"
  echo "git -C ${arg_slug} submodule update --init --recursive --depth 1" >> "${rerun}"
  echo "singularity shell docker://${arg_username}/${arg_slug}@sha256:${dockerfile_sha} \
<< END_OF_HEREDOC
${INPUT}
END_OF_HEREDOC" >> "${rerun}"
  chmod +x "${rerun}"

  echo "uploading rerun script"
  osf -p $arg_project upload \
    "${rerun}" \
    "repro/${REPRO_ID}/a=rerun+repro=${REPRO_ID}+ext=.sh"

  echo "uploading log"
  osf -p $arg_project upload \
    "${log}" \
    "repro/${REPRO_ID}/a=log+repro=${REPRO_ID}+ext=.txt"

  echo "on exit cleanup complete"

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
  bash <(curl https://raw.githubusercontent.com/${arg_username}/pushover.sh/master/pushover.sh) \
   -T "$PUSHOVER_APP_TOKEN" -U "$PUSHOVER_USER_TOKEN" \
   -u $(osf -p $arg_project geturl \
      "repro/${REPRO_ID}/a=log+repro=${REPRO_ID}+ext=.txt" \
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
echo "Generate REPRO_ID"
echo "--------------------------------------"
################################################################################

export REPRO_ID=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 16 | head -n 1)
echo "REPRO_ID ${REPRO_ID}"

################################################################################
echo
echo "Setup Work Directory"
echo "--------------------------------------"
################################################################################
rm -rf "${REPRO_ID}" && mkdir "${REPRO_ID}" && cd "${REPRO_ID}"
pwd

################################################################################
echo
echo "Get Assets"
echo "--------------------------------------"
################################################################################

# setup output folder
mkdir -p output

# setup latest project source
if [ -n "${repo_sha}" ]; then
  mkdir "${arg_slug}"
  git -C "${arg_slug}" init
  git -C "${arg_slug}" remote add origin "https://github.com/${arg_username}/${arg_slug}.git"
  git -C "${arg_slug}" fetch --depth 1 origin "${repo_sha}"
  git -C "${arg_slug}" checkout FETCH_HEAD
  git -C "${arg_slug}" submodule update --init --recursive --depth 1
else
  git clone "https://github.com/${arg_username}/${arg_slug}.git" \
    --depth 1 --recursive --shallow-submodules \
    --branch "${arg_branch}"
  repo_sha=$(git -C "${arg_slug}" rev-parse HEAD)
fi

echo "repo_sha ${repo_sha}"

# get sha of latest docker container
if [ -z "${container_sha}" ]; then
  container_sha=$(\
    singularity exec "docker://${arg_username}/${arg_slug}" \
      bash -c 'echo ${SINGULARITY_NAME}' \
  )
fi
echo "container_sha ${container_sha}"

################################################################################
echo
echo "Do Work"
echo "--------------------------------------"
################################################################################

# pipe input into the container, record a copy to $STDIN
tee "${stdin}" | \
  singularity shell --env "SECONDS=${SECONDS}" "docker://${arg_username}/${arg_slug}@sha256:${container_sha}"
