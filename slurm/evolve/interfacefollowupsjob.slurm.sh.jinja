#!/bin/bash -login
########## Define Resources Needed with SBATCH Lines ##########
#SBATCH --time=4:00:00
#SBATCH --job-name a=interfacefollowups+series={{ series }}+stint={{ stint }}
#
#SBATCH --output="/mnt/scratch/%u/slurmlogs/a=interfacefollowups+job=%j+series={{ series }}+stint={{ stint }}+ext.log"
#SBATCH --mem=24G
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 4
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=mawni4ah2o@pomail.net
#SBATCH --account=ecode
# No --mail-user, the default value is the submitting user
#
# Send interrupt when within 5 minutes of end time.
#SBATCH --signal=SIGINT@300
# Job may be requeued after node failure.
#SBATCH --requeue

################################################################################
echo
echo "running interfacefollowupsjob.slurm.sh"
echo "--------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

# specify display, start the Xvfb server, and save the process ID
# adapted from https://wiki.hpcc.msu.edu/display/~colbrydi@msu.edu/2013/10
export DISPLAY=":${SLURM_JOB_ID-1}"
rm -f "/tmp/.X11-unix/X${SLURM_JOB_ID-1}"
rm -f "/tmp/.X${SLURM_JOB_ID-1}-lock"
Xvfb "${DISPLAY}" -auth /dev/null/ &
export XVFB_PID=$!

# set up and jump into temporary work directory
cd "$(mktemp -d)"

# curl repro_runner.sh script into to a temporary file
REPRO_RUNNER="$(mktemp)"
for retry in {1..20}; do
  curl \
    -o "${REPRO_RUNNER}" \
    "https://raw.githubusercontent.com/mmore500/dishtiny/{{ repo_sha }}/script/repro_runner.sh" \
  && echo "  repro_runner curl success" && break \
  || (echo "retrying repro_runner curl (${retry})" && sleep $((RANDOM % 10)))
  if ((${retry}==20)); then echo "repro_runner curl fail" && exit 123123; fi
done
chmod +x "${REPRO_RUNNER}"

################################################################################
echo
echo "Run Job with repro_runner.sh"
echo "--------------------------------"
################################################################################

"${REPRO_RUNNER}" \
  -p "{{ bucket }}" -u mmore500 -s dishtiny \
  --repo_sha "{{ repo_sha }}" --container_tag "{{ container_tag }}" \
  << 'REPRO_RUNNER_HEREDOC_EOF'

################################################################################
echo
echo "running interfacefollowupsjob.slurm.sh"
echo "--------------------------"
################################################################################

# fail on error
set -e
# adapted from https://unix.stackexchange.com/a/504829
printerr() {
    echo "Error occurred:"
    awk 'NR>L-4 && NR<L+4 { printf "%-5d%3s%s\n",NR,(NR==L?">>>":""),$0 }' L=$1 $0
}
trap 'printerr $LINENO' ERR

################################################################################
echo
echo "Initialize and Log Config"
echo "-------------------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

# load secrets into environment variables, if available
[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

ENDEAVOR={{ series | int // 1000 }}
GENESIS="{{ "generate" if stint | int == 0 else "reconstitute" }}"
N_PROCS=1
N_THREADS=4

echo "ENDEAVOR ${ENDEAVOR}"
echo "GENESIS ${GENESIS}"
echo "N_PROCS ${N_PROCS}"
echo "N_THREADS ${N_THREADS}"
echo "series {{ series }}"
echo "stint {{ stint }}"
echo "configpack {{ configpack }}"
echo "container_tag {{ container_tag }}"
echo "repo_sha {{ repo_sha }}"
echo "followup_freq {{ followup_freq | default('10') }}"

################################################################################
echo
echo "Set Up Work Directory"
echo "-------------------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

mkdir work
cd work

bash -c "cd ../dishtiny && ./configpacks/{{ configpack }}/build_native.sh"
cp ../dishtiny/rundishtiny .

ls

# an attempt to address intermittent failure where downloaded files aren't found
# when running the executable
sshpass -p "${HOST_PASSWORD}" \
  scp -r -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
    * "${HOST_USERNAME}@$(hostname):$(pwd)" \
&& echo "  local asset copy success" || echo "  local asset copy fail"

ls

################################################################################
echo
echo "Submit Perturbation Competition Jobs"
echo "-----------------------------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

# run mutating competition job every tenth stint
if (( {{ stint }} % {{ followup_freq | default('10') }} == 0 )); then

for perturbation_target in extrospective introspective writable; do
for perturbation_method in exchange rotate; do

echo "perturbation_target ${perturbation_target}"
echo "perturbation_method ${perturbation_method}"

for retry in {1..20}; do
  # adapted from https://backreference.org/2011/08/10/running-local-script-remotely-with-arguments/
  ( echo "export SLURM_STOKER_CONSOLIDATION_DIR=\"${SLURM_STOKER_CONSOLIDATION_DIR}\""; echo "export REPRO_ID=\"${REPRO_ID}\""; wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/{{ repo_sha }}/slurm/competition/perturbationcompetitionkickoff.sh" ) | sshpass -p "${HOST_PASSWORD}" ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
    "${HOST_USERNAME}@$(hostname)" -X \
    "cat | bash -login /dev/stdin '{{ bucket }}' '{{ configpack }}' '{{ container_tag }}' '{{ repo_sha }}' \"${perturbation_target}\" \"${perturbation_method}\" 1 \"{0..$(./rundishtiny "-print_${perturbation_target}_state_size_minus_one" )}\" '{{ stint }}' '{{ series }}'" \
    && echo "  perturbation competition kickoff success" && break \
    || (echo "retrying perturbation competition kickoff (${retry})" && sleep $((RANDOM % 10)))

  if ((${retry}==20)); then
    echo "perturbation competition kickoff fail" && exit 123123
  fi
done

done
done

fi # if (( {{ stint }} % {{ followup_freq | default('10') }} == 0 )); then

################################################################################
echo
echo "Submit SelfSend Competition Jobs"
echo "-----------------------------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

# run mutating competition job every tenth stint
if (( {{ stint }} % {{ followup_freq | default('10') }} == 0 )); then

for selfsend_target in inter intra; do

echo "selfsend_target ${selfsend_target}"

for retry in {1..20}; do
  # adapted from https://backreference.org/2011/08/10/running-local-script-remotely-with-arguments/
  ( echo "export SLURM_STOKER_CONSOLIDATION_DIR=\"${SLURM_STOKER_CONSOLIDATION_DIR}\""; echo "export REPRO_ID=\"${REPRO_ID}\""; wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/{{ repo_sha }}/slurm/competition/selfsendcompetitionkickoff.sh" ) | sshpass -p "${HOST_PASSWORD}" ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile /dev/null" \
    "${HOST_USERNAME}@$(hostname)" -X \
    "cat | bash -login /dev/stdin '{{ bucket }}' '{{ configpack }}' '{{ container_tag }}' '{{ repo_sha }}' \"${selfsend_target}\" 20 '{{ stint }}' '{{ series }}'" \
    && echo "  selfsend competition kickoff success" && break \
    || (echo "retrying selfsend competition kickoff (${retry})" && sleep $((RANDOM % 10)))

  if ((${retry}==20)); then
    echo "selfsend competition kickoff fail" && exit 123123
  fi
done

done

fi # if (( {{ stint }} % {{ followup_freq | default('10') }} == 0 )); then

################################################################################
echo
echo "Submit slurm scripts from SLURM_STOKER_CONSOLIDATION_DIR"
echo "-----------------------------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

echo "${SLURM_STOKER_CONSOLIDATION_DIR}"
cd "${SLURM_STOKER_CONSOLIDATION_DIR}"
pwd

# if there are follow up jobs
if ls *.slurm.sh 1> /dev/null 2>&1; then

wget --retry-connrefused --waitretry=1 --read-timeout=20 --timeout=15 -t 10 -qO- "https://raw.githubusercontent.com/mmore500/dishtiny/{{ repo_sha }}/script/slurm_stoker_containerized_kickoff.sh" | bash -s "{{ bucket }}" "{{ container_tag }}" "{{ repo_sha }}" "evolvejob-consolidated-followups~series%{{ series }}~stint%{{ stint }}"

else

echo "no follow up jobs to submit"

fi # if (( {{ stint }} % {{ followup_freq | default('10') }} == 0 )); then

cd -

################################################################################
echo
echo "Done! (SUCCESS)"
echo "---------------"
################################################################################

timeout 30s "${REPRO_WORK_DIRECTORY}/dishtiny/script/host_squeue.sh" -j "${SLURM_JOB_ID}" --format \"time: %M, time left: %L\" --noheader || echo "time log failed" &
wait # ignores exit code of time log step

REPRO_RUNNER_HEREDOC_EOF
