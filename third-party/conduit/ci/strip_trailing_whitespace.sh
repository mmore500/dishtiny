#!/bin/bash

TARGETS=$(find . -type f ! -path "./third-party/*" ! -path "./.git/*" ! -path "*/assets/*" ! -path "./node_modules/*")

for filename in $TARGETS
do
  # end of files
  # adapted from https://unix.stackexchange.com/a/81687
  sed -i -e :a -e '/^\n*$/{$d;N;};/\n$/ba' $filename

  # end of lines
  sed -i 's/\s*$//g' $filename
done
