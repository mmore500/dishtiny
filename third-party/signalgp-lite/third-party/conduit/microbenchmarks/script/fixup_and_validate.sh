#!/bin/bash

for filename in $(find . -name "*.json"); do
  echo "$filename"
  sed -i "s/: Infinity,/: null,/g w /dev/stdout" "$filename" | wc -l
  sed -i "s/: -NaN,/: null,/g w /dev/stdout" "$filename" | wc -l
  sed -i "s/: NaN,/: null,/g w /dev/stdout" "$filename" | wc -l
  sed -i "s/: -nan,/: null,/g w /dev/stdout" "$filename" | wc -l
  sed -i "s/: nan,/: null,/g w /dev/stdout" "$filename" | wc -l
  jsonlint --quiet "$filename"
done
