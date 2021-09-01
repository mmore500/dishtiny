#!/bin/bash

SOURCE_HASH=$( find -path ./third-party -prune -false -o -type f | sort | xargs cat | sha1sum )

./binder/clear_notebooks.sh
./binder/clear_outplots.sh

if [ "${SOURCE_HASH}" == "$( find -path ./third-party -prune -false -o -type f | sort | xargs cat | sha1sum )" ];
then
  exit 0 # success
else
  echo "dirty notebooks detected, run ./binder/clear_notebooks.sh ./binder/clear_outplots.sh locally to find & fix"
  exit 1 # failure
fi
