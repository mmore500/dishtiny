#!/bin/bash

running_workjobs="$(
  squeue -u "$(whoami)" --format "%t" \
  | grep -v "a=slurm_stoker+" \
  | grep R \
  | wc -l \
)"
echo "running workjobs ${running_workjobs}"

mean_workjob_elapsed_time="$(
  squeue -u "$(whoami)" --format "%M %t %j" \
  | grep -v "a=slurm_stoker+" \
  | grep R \
  | awk '{print $1}' \
  | python3 -c '\
    import datetime; import sys; \
    deltafy = lambda timestring: datetime.timedelta( **dict(zip( \
        ["seconds", "minutes", "hours", "days"], \
        map(int, reversed(timestring.split(":"))), \
    )) ); \
    input = list(sys.stdin); \
    res = sum( map(deltafy, input), datetime.timedelta() ) / len(input); \
    print( res - datetime.timedelta(microseconds=res.microseconds) )'
)"
echo "mean workjob elapsed time ${mean_workjob_elapsed_time}"

mean_workjob_elapsed_secs="$(
  squeue -u "$(whoami)" --format "%M %t %j" \
  | grep -v "a=slurm_stoker+" \
  | grep R \
  | awk '{print $1}' \
  | python3 -c '\
    import datetime; import sys; \
    deltafy = lambda timestring: datetime.timedelta( **dict(zip( \
        ["seconds", "minutes", "hours", "days"], \
        map(int, reversed(timestring.split(":"))), \
    )) ); \
    input = list(sys.stdin); \
    res = sum( map(deltafy, input), datetime.timedelta() ) / len(input); \
    print( int( res.total_seconds() ) )'
)"
echo "mean workjob elapsed secs ${mean_workjob_elapsed_secs}"

echo

queued_workjobs="$(
  squeue -u "$(whoami)" --format "%j %T %R" \
  | grep -v "a=slurm_stoker+" \
  | grep -v "RUNNING" \
  | grep -v "JobHeldUser" \
  | wc -l \
)"
echo "queued workjobs ${queued_workjobs}"

slurmstoked_workjobs="$( \
  squeue -u "$(whoami)" --format "%j %R" \
  | grep "a=slurm_stoker+" \
  | grep -v "JobHeldUser" \
  | grep -Po 'num_jobs=\K[0-9]+?(?=\+)' \
  | paste -sd+ - \
  | bc \
)"
echo "slurmstoked workjobs ${slurmstoked_workjobs}"

pending_workjobs=$((${slurmstoked_workjobs} + ${queued_workjobs}))
echo "pending workjobs ${pending_workjobs}"

expected_seconds=$(( \
  ${mean_workjob_elapsed_secs} \
  * 2 \
  * ${pending_workjobs} \
  / ${running_workjobs} \
))

printf 'expected remaining time %dd:%dh:%dm:%ds\n' \
  $((expected_seconds/86400)) \
  $((expected_seconds%86400/3600)) \
  $((expected_seconds%3600/60)) \
  $((expected_seconds%60))

echo

jobtypes="$( \
  squeue -u "$(whoami)" --format %j \
  | grep "a=slurm_stoker+" \
  | grep -Po 'title=\K.*?(?=~)' \
  | sort \
  | uniq \
)"
for jobtype in ${jobtypes}; do
  echo -n "stoked ${jobtype} jobs "
  squeue -u "$(whoami)" --format %j \
  | grep "a=slurm_stoker+" \
  | grep "${jobtype}" \
  | grep -Po 'num_jobs=\K[0-9]+?(?=\+)' \
  | paste -sd+ - \
  | bc
done
