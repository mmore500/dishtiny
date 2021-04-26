#!/bin/bash

# usage ./check_quota.sh

command -v /usr/local/hpcc/bin/display_user_quota.pl >/dev/null \
  || { echo "no quota command, skipping"; exit 0; }

# exit code success if quota okay
# exit code fail if quota close to exceeded

quota_result="$( /usr/local/hpcc/bin/display_user_quota.pl )"

# get home line
# then condense whitespace
# then get fourth word
# then strip non-alphanumeric characters
HOME_SPACE_PERCENT_USED="$( \
  echo "${quota_result}" \
  | grep /mnt/home \
  | tr -s " " \
  | cut -d " " -f 5 \
  | tr -cd '[:alnum:]._-' \
)"
echo "HOME_SPACE_PERCENT_USED ${HOME_SPACE_PERCENT_USED}"

HOME_FILES_PERCENT_USED="$( \
  echo "${quota_result}" \
  | grep /mnt/home \
  | tr -s " " \
  | cut -d " " -f 9 \
  | tr -cd '[:alnum:]._-' \
)"
echo "HOME_FILES_PERCENT_USED ${HOME_FILES_PERCENT_USED}"

SCRATCH_SPACE_PERCENT_USED="$( \
  echo "${quota_result}" \
  | grep -A1 /mnt/scratch \
  | tail -n1 \
  | tr -s " " \
  | cut -d " " -f 5 \
  | tr -cd '[:alnum:]._-' \
)"
echo "SCRATCH_SPACE_PERCENT_USED ${SCRATCH_SPACE_PERCENT_USED}"

SCRATCH_FILES_PERCENT_USED="$( \
  echo "${quota_result}" \
  | grep -A1 /mnt/scratch \
  | tail -n1 \
  | tr -s " " \
  | cut -d " " -f 9 \
  | tr -cd '[:alnum:]._-' \
)"
echo "SCRATCH_FILES_PERCENT_USED ${SCRATCH_FILES_PERCENT_USED}"

exit_code=$(( \
  (HOME_SPACE_PERCENT_USED > 80) \
  || (HOME_FILES_PERCENT_USED > 80) \
  || (SCRATCH_SPACE_PERCENT_USED > 80 ) \
  || (SCRATCH_FILES_PERCENT_USED > 80 ) \
))

echo "exit_code ${exit_code}"

# convert string to integer
exit $((exit_code))
