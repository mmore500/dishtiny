#!/bin/bash

SOURCE_HASH=$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )

./ci/replace_tabs.sh

if [ "${SOURCE_HASH}" == "$( find -path ./third-party -prune -false -o -type f | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  echo "tab characters detected, run ./ci/replace_tabs.sh locally to find & fix"
  exit 1 # failure
fi
