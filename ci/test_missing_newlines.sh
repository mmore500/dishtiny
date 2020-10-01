#!/bin/bash

SOURCE_HASH=$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )

./ci/fix_missing_newlines.sh

if [ "${SOURCE_HASH}" == "$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  echo "missing newline at end of file detected, run ./ci/fix_missing_newlines.sh locally to find & fix"
  exit 1 # failure
fi
