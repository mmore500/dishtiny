#!/bin/bash

TARGETS=$(find . -type f \( -name "*.hpp" -o -name "*.cpp" \) ! -path "./third-party/*")

for filename in $TARGETS
do

  printf "."

  # adapted from https://stackoverflow.com/a/6970681
  include_linenos=$(awk '/^#include .*/ {print FNR}' $filename)

  # adapted from https://stackoverflow.com/a/26809816
  # and https://superuser.com/a/284192
  echo $include_linenos | tr ' ' '\n' | awk 'NR==1{first=$1;last=$1;next} $1 == last+1 {last=$1;next} {print first,last;first=$1;last=first} END{print first,last}' | while read line ; do

    # adapted from https://unix.stackexchange.com/a/11064
    read start stop <<< $line

    # adapted from https://stackoverflow.com/a/46018238
    echo "x" | ex -s -c "${start},${stop}!sort" $filename

  done
done

echo
