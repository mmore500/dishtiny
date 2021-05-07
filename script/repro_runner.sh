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

  9. if the shell session exited an error code, a pushover notification is dispatched
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
  -c / --container_tag tag of docker container to run in, can include container sha (default: latest)
  -h / --help  show this message and exit
  -r / --repo_sha repo sha to check out (default: none)

exported environment variables:
  REPRO_ID a randomly generated 16-character string identifying this session
  REPRO_YEAR
  REPRO_MONTH
  REPRO_DAY
  REPRO_HOUR

"

################################################################################
echo
echo "Setup Temporary Files"
echo "--------------------------------------"
################################################################################

stdin=$(mktemp)
log=$(mktemp)
logzip=$(mktemp)
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
echo "running repro_runner.sh"
echo "-----------------------"
################################################################################

echo "${0} ${@}"

################################################################################
echo
echo "Parse Arguments"
echo "--------------------------------------"
################################################################################

# adapted from https://stackoverflow.com/a/33826763
while [[ "$#" -gt 0 ]]; do
  case $1 in
    -b|--branch) arg_branch="$2"; shift ;; # source branch to check out
    -c|--container_tag) container_tag="$2"; shift ;; # container to run in
    -h|--help) echo "$usage"; exit;; # print help message
    -p|--project) arg_project="$2"; shift ;; # osf project id
    -r|--repo_sha) repo_sha="$2"; shift ;; # source sha to check out
    -s|--slug) arg_slug="$2"; shift ;; # github/dockerhub project slug
    -u|--username) arg_username="$2"; shift ;; # github/dockerhub username
    *) echo "Unknown argument passed: $1"; exit 1 ;;
  esac
  shift
done

# setup default arguments
test ${arg_branch} || arg_branch=master
test ${container_tag} || container_tag=latest

echo "arg_branch ${arg_branch}"
echo "container_tag ${container_tag}"
echo "arg_project ${arg_project}"
echo "repo_sha ${repo_sha}"
echo "arg_slug ${arg_slug}"
echo "arg_username ${arg_username}"

################################################################################
echo
echo "Assert Prequisites"
echo "--------------------------------------"
################################################################################

command -v module >/dev/null && module load git/2.27.0 && module list || :
git --version
[[ -f ~/pyenv/bin/activate ]] && source ~/pyenv/bin/activate
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh

command -v osf >/dev/null || ( echo "no osf client" && exit 1 )
test ${OSF_USERNAME} || ( echo "no OSF_USERNAME env var" && exit 1 )
test ${OSF_PASSWORD} || ( echo "no OSF_PASSWORD env var" && exit 1 )
test ${PUSHOVER_USER_TOKEN} || ( echo "no PUSHOVER_USER_TOKEN env var" && exit 1 )
test ${PUSHOVER_APP_TOKEN} || ( echo "no PUSHOVER_APP_TOKEN env var" && exit 1 )
test ${arg_project} || ( echo "no --project arg" && exit 1 )
test ${arg_slug} || ( echo "no --slug arg" && exit 1 )
test ${arg_username} || ( echo "no --username arg" && exit 1 )


################################################################################
echo
echo "Check quota"
echo "--------------------------------------"
################################################################################

# adapted from https://superuser.com/a/689340
# and https://stackoverflow.com/a/4642975
wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/master/script/check_quota.sh" | bash \
|| wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/pushover.sh/master/pushover.sh" | bash -s  \
  -T "${PUSHOVER_APP_TOKEN}" -U "${PUSHOVER_USER_TOKEN}" \
  "check_quota.sh fail $(/usr/local/hpcc/bin/display_user_quota.pl)"


################################################################################
echo
echo "Testing for existing repro_runner.sh session"
echo "--------------------------------------------"
################################################################################
if test -v REPRO_ID; then

echo "REPRO_ID already set"
echo "REPRO_ID ${REPRO_ID}"
echo "(invoked from inside another repro_runner.sh session)"
echo "skipping exit and error trap setup"

else # if test -v REPRO_ID; then

################################################################################
echo
echo "Generate REPRO_ID"
echo "--------------------------------------"
################################################################################

export REPRO_ID="$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 16 | head -n 1)"
export REPRO_YEAR="$(date +'%Y')"
export REPRO_MONTH="$(date +'%m')"
export REPRO_DAY="$(date +'%d')"
export REPRO_HOUR="$(date +'%H')"
export REPRO_DIR="$(pwd)"

echo "REPRO_YEAR ${REPRO_YEAR}"
echo "REPRO_MONTH ${REPRO_MONTH}"
echo "REPRO_DAY ${REPRO_DAY}"
echo "REPRO_HOUR ${REPRO_HOUR}"
echo "REPRO_ID ${REPRO_ID}"
echo "REPRO_DIR ${REPRO_DIR}"

REPRO_PATH="a=repro+year=${REPRO_YEAR}/month=${REPRO_MONTH}/day=${REPRO_DAY}/hour=${REPRO_HOUR}/id=${REPRO_ID}"
echo "REPRO_PATH ${REPRO_PATH}"

################################################################################
echo
echo "Log Job Info"
echo "--------------------------------------"
################################################################################

echo "SLURM_JOB_ID ${SLURM_JOB_ID}"
echo "SLURM_JOB_NAME ${SLURM_JOB_NAME}"
echo "SLURM_RESTART_COUNT ${SLURM_RESTART_COUNT}"

echo "REPRO_ID ${REPRO_ID}"
echo "date $(date)"
echo "hostname $(hostname)"
echo "pwd $(pwd)"
command -v qstat >/dev/null && test ${SLURM_JOB_ID} && bash -c "qstat -f \"${SLURM_JOB_ID}\""

echo "stdin ${stdin}"
echo "log ${log}"
echo "logzip ${logzip}"
echo "context ${context}"
echo "rerun ${rerun}"
echo "output ${output}"
echo "manifest ${manifest}"
echo "runner ${runner}"

################################################################################
echo
echo "Setup Exit and Error Traps"
echo "--------------------------------------"
################################################################################

function on_exit {

  echo
  echo "Run Exit Trap"
  echo "-------------"

  echo "SLURM_RESTART_COUNT ${SLURM_RESTART_COUNT}"

  echo "log job duration..."
  echo "  date $(date)"
  echo "  SECONDS ${SECONDS}"

  # echo "uploading stdin"
  # for retry in {1..3}; do
  #   osf -p "${arg_project}" upload \
  #     "${stdin}" \
  #     "${REPRO_PATH}/a=stdin+repro=${REPRO_ID}+ext=.txt" \
  #   && echo "  stdin upload success" \
  #   && break \
  #     || (echo "retrying stdin upload (${retry})" && sleep $((RANDOM % 10)))
  #   if ((${retry}==3)); then echo "upload stdin fail"; fi
  # done &

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
  echo "singularity shell docker://${arg_username}/${arg_slug}@${container_tag#*@} \
<< 'END_OF_HEREDOC'
${INPUT}
END_OF_HEREDOC" >> "${rerun}"
  chmod +x "${rerun}"

  # echo "uploading rerun"
  # for retry in {1..3}; do
  #   osf -p "${arg_project}" upload \
  #     "${rerun}" \
  #     "${REPRO_PATH}/a=rerun+repro=${REPRO_ID}+ext=.sh" \
  #   && echo "  rerun upload success" \
  #   && break \
  #     || (echo "retrying rerun upload (${retry})" && sleep $((RANDOM % 10)))
  #   if ((${retry}==3)); then echo "upload rerun fail"; fi
  # done &

  xz -c -9e "${log}" > "${logzip}"
  echo "uploading log"
  for retry in {1..3}; do
    aws s3 cp --quiet \
      "${logzip}" \
      "s3://${arg_project}/repro/a=log+repro=${REPRO_ID}+ext=.txt.xz" \
    && echo "  log upload success" \
    && break \
      || (echo "retrying log upload (${retry})" && sleep $((RANDOM % 10)))
    if ((${retry}==3)); then echo "upload log fail"; fi
  done &

  # if user has created an output directory, upload it
  # if [ -d "${WORK_DIRECTORY}/output" ]; then
  #
  #   echo "uploading output manifest"
  #   ls "${WORK_DIRECTORY}/output" > "${manifest}"
  #   for retry in {1..3}; do
  #     osf -p "${arg_project}" upload \
  #       "${manifest}" \
  #       "${REPRO_PATH}/a=manifest+repro=${REPRO_ID}+ext=.txt" \
  #     && echo "  manifest upload success" \
  #     && break \
  #       || (echo "retrying manifest upload (${retry})" && sleep $((RANDOM % 10)))
  #     if ((${retry}==3)); then echo "upload manifest fail"; fi
  #   done &
  #
  #   echo "uploading output"
  #   tar -czf "${output}" "${WORK_DIRECTORY}/output"
  #   for retry in {1..10}; do
  #     osf -p "${arg_project}" upload \
  #       "${output}" \
  #       "${REPRO_PATH}/a=output+repro=${REPRO_ID}+ext=.tar.gz" \
  #     && echo "  output upload success" \
  #     && break \
  #       || (echo "retrying output upload (${retry})" && sleep $((RANDOM % 10)))
  #     if ((${retry}==10)); then echo "upload output fail"; fi
  #   done
  #
  #   raw_output_url=$(osf -p "${arg_project}" geturl \
  #     "${REPRO_PATH}/a=output+repro=${REPRO_ID}+ext=.tar.gz" \
  #   )
  #   output_url=$(curl -Ls -o /dev/null -w %{url_effective} $raw_output_url)
  #   echo "output uploaded to ${output_url}"
  #   echo "  download a copy: curl -L ${output_url}download --output ${REPRO_ID}.tar.gz"
  #
  # fi

  wait

  test -v SLURM_JOB_ID && command -v scontrol \
    && mkdir -p "/mnt/scratch/$(whoami)/slurmscripts/" \
    && scontrol write batch_script "${SLURM_JOB_ID}" "/mnt/scratch/$(whoami)/slurmscripts/slurm-${SLURM_JOB_ID}.sh" \
    && echo "wrote current slurm script to /mnt/scratch/$(whoami)/slurmscripts"

  echo "Exit Trap Complete"

}

function on_error() {

  echo
  echo "Run Error Trap (FAIL)"
  echo "---------------------"

  echo "   EXIT STATUS ${1}"
  echo "   LINE NO ${2}"

  echo "---------------------"
  echo

  # don't exit on further failures
  set +e

  # adapted from https://unix.stackexchange.com/a/504829
  awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$2 $0

  for target in $(find . -name '*a=log+*'); do
    echo "uploading ${target}"
    proc="$(echo "${target}" | keyname extract proc)"
    echo "proc ${proc}"
    thread="$(echo "${target}" | keyname extract thread)"
    echo "thread ${thread}"
    # renaming excludes characters that cause url headaches
    curl --upload-file "${target}" "https://transfer.sh/a=log+job=${SLURM_JOB_ID}+proc=${proc}+thread=${thread}+ext=.txt"
    echo
  done

  echo "sstat -j ${SLURM_JOB_ID}"
  sstat -j "${SLURM_JOB_ID}"

  echo "scontrol show jobid -dd ${SLURM_JOB_ID}"
  scontrol show jobid -dd "${SLURM_JOB_ID}"

  echo "squeue -u $(whoami) | wc -l" && squeue -u "$(whoami)" | wc -l
  echo "squeue -u $(whoami)" && squeue -u "$(whoami)"

  quota

  echo "hostname" && hostname
  echo "lscpu" && lscpu
  echo "date" && date

  echo "pwd" && pwd
  echo "ls" && ls
  echo "du -h .  --max-depth 1" && du -h . --max-depth 1
  echo "tree --du -h out*" && tree --du -h . || :

  # memory info
  echo "cat /proc/meminfo" && cat /proc/meminfo
  echo "free -g" && free -g
  echo "vmstat" && vmstat

  # disk info
  echo "lsblk" && lsblk
  echo "df -h" && df -h
  echo "df -i" && df -i
  # echo "ioping -c /tmp" && ioping -c /tmp
  # echo "ioping -c ." && ioping -c .
  # echo "ioping -c ~" && ioping -c ~

  # network info
  echo "curl -I https://google.com" && curl -I https://google.com
  echo "ipcs" && ipcs
  echo "cat /proc/net/dev" && cat /proc/net/dev
  # time curl -o /dev/null http://speedtest-blr1.digitalocean.com/10mb.test

  # upload log files, with weird hack to prevent premature exit
  on_exit &
  wait

  echo "Sending Pushover Notification"
  bash <(curl https://raw.githubusercontent.com/mmore500/pushover.sh/master/pushover.sh) \
    -u $(curl --upload-file \
      "${log}" \
      "https://transfer.sh/a=log+repro=${REPRO_ID}+ext=.txt" \
    ) \
   -T "$PUSHOVER_APP_TOKEN" -U "$PUSHOVER_USER_TOKEN" \
    "${SLURM_JOB_ID} ${SLURM_JOB_NAME} error code ${1}, restart count ${SLURM_RESTART_COUNT}"

  # reset exit trap
  trap '' EXIT

  if test -v SLURM_RESTART_COUNT; then
    echo "job requeue failure, job has already been requeued"
    echo "SLURM_RESTART_COUNT ${SLURM_RESTART_COUNT}"
  elif test -v SLURM_JOB_ID; then

    # preserve existing attempt's logfile
    # which will be overwritten by the requeue'd job
    slurm_logfile="$(shopt -s nullglob; ls "/mnt/scratch/$(whoami)/slurmlogs/"*${SLURM_JOB_ID}*)"
    echo "slurm_logfile ${slurm_logfile}"

    for file in ${slurm_logfile}; do
      echo "file ${file}"
      cp "${file}" "${file}.bak${SLURM_RESTART_COUNT}"
    done

    # job will be cancelled immediately upon requeue
    command -v scontrol \
    && echo "attempting job requeue" \
    && scontrol requeue "${SLURM_JOB_ID}" \
    && echo "job requeue success" \
    || echo "job requeue failure, requeue error"

  else
    echo "not a slurm job, not attempting requeue"
  fi

  echo "Error Trap Complete"

  exit ${1}

}

trap 'on_error $? $LINENO' ERR SIGINT
trap on_exit EXIT

fi # if test -v REPRO_ID; then

################################################################################
echo
echo "Setup Work Directory"
echo "--------------------------------------"
################################################################################
rm -rf "${REPRO_ID}" && mkdir "${REPRO_ID}" && cd "${REPRO_ID}"
export WORK_DIRECTORY="$(pwd)"
export REPRO_WORK_DIRECTORY="${WORK_DIRECTORY}"
echo "WORK_DIRECTORY ${WORK_DIRECTORY}"

################################################################################
echo
echo "Get sha tag for docker container"
echo "--------------------------------------"
################################################################################

if [[ "${container_tag}" != *"@sha256:"* ]]; then

  # use several retries to fix intermittent TLS handshake timeout
  container_tag="${container_tag}@sha256:$(\
    until singularity exec "docker://${arg_username}/${arg_slug}:${container_tag}" \
      bash -c 'echo ${SINGULARITY_NAME}' \
    || (( retry++ >=5 )); do sleep $(( retry * retry * 60 )); done \
  )"
fi
echo "container_tag with sha256 ${container_tag}"


################################################################################
echo
echo "Get Assets"
echo "--------------------------------------"
################################################################################

# use several retries to fix intermittent TLS handshake timeout
container_file="$(mktemp)"
retry=0
time until singularity pull --force "${container_file}" "docker://${arg_username}/${arg_slug}@${container_tag#*@}"; do
  (( retry++ >= 5 )) && echo "too many singularity pull retries" && exit 1
  echo "failed singularity pull ${retry}, trying agian" && sleep $(( retry * retry * 60 ))
done

# setup output folder
# no, user has to make output folder if they want it
# mkdir -p output

echo "repo_sha before asset get ${repo_sha}"

if [ -n "${repo_sha}" ]; then

  time wget \
    --retry-connrefused --waitretry=1 --read-timeout=20 \
    --timeout=15 -t 10 -qO- \
    "https://raw.githubusercontent.com/mmore500/dishtiny/${repo_sha}/script/gitget.sh" \
  | bash -s \
    "https://github.com/${arg_username}/${arg_slug}.git" \
    "${arg_slug}" \
    "${repo_sha}"

else
  echo "setting up latest project source..."
  time for retry in {1..20}; do
    rm -rf "${arg_slug}" \
    && git clone "https://github.com/${arg_username}/${arg_slug}.git" \
      --quiet --depth 1 --jobs 16 --recursive --shallow-submodules \
      --branch "${arg_branch}" \
    && echo "  source clone success" \
    && break \
    || (echo "source clone failure (${retry})" && sleep $((RANDOM % 10)))

    if ((${retry}==20)); then echo "source clone fail" && exit 123123; fi
  done
  repo_sha=$(git -C "${arg_slug}" rev-parse HEAD)
fi

echo "repo_sha after asset get ${repo_sha}"

################################################################################
echo
echo "Do Work"
echo "--------------------------------------"
################################################################################

# Timeout with 4 minutes (240 seconds) left in job.
# If not a slurm job, set duration to 0 to disable the associated timeout.
TIMEOUT_SECONDS=$( \
  test ${SLURM_JOB_ID} \
  && squeue -j "${SLURM_JOB_ID}" -O "TimeLeft" \
    | tail -n 1 \
    | tr ":" "\n" \
    | tac \
    | tr "\n" " " \
    |  awk '{ print $1 + $2*60 + $3*3600 + $4*86400 }' \
    | awk '{print $1-240}' \
  || echo 0 \
)
echo "TIMEOUT_SECONDS ${TIMEOUT_SECONDS}"


# pipe input into the container, record a copy to $STDIN
# Docker references with both a tag and digest are currently not supported in
# singularity so we have to strip everything before @sha256:digest
cat "-" | timeout "${TIMEOUT_SECONDS}s" tee "${stdin}" | \
  singularity shell \
    --env "SECONDS=${SECONDS}" \
    -B "${HOME}:/home/user" \
    --dns 8.8.8.8,8.8.4.4 \
    "${container_file}"

# not critical clean up, so not bothering with exit trap
rm -f "${container_file}"
