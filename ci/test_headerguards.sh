#!/bin/bash

SOURCE_HASH=$( find include -type f | sort | xargs cat | sha1sum )

./ci/generate_headerguards.sh

if [ "${SOURCE_HASH}" == "$( find include -type f | sort | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  echo "inconsistent or missing header guards detected, run ./ci/generate_headerguards.sh locally to find & fix"
  exit 1 # failure
fi
