#!/bin/bash

# greq -qI tests if file is binary
# adapted from https://stackoverflow.com/a/29689345
TARGETS=$(find . -type f ! -path "./third-party/*" ! -path "./.git/*" ! -path "*/assets/*" ! -path "*.tar.gz" ! -path "*.jpg" ! -path "*.png" ! -path "*Makefile" ! -path "*Maketemplate*" -exec grep -qI . {} ';' -print)

for filename in $TARGETS
do
  sed -i 's/\t/  /g' "$filename"
done
