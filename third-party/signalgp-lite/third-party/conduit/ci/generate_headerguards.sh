#!/bin/bash

for filename in $(cd include && find -- * -name '*.hpp' -type f); do

  GUARD=$( sed "s/[^[:alnum:]]/_/g" <<< "$filename" | tr [a-z] [A-Z] )
  NDEF_LINE="#ifndef ${GUARD}_INCLUDE"
  DEF_LINE="#define ${GUARD}_INCLUDE"
  ENDIF_LINE="#endif // ${NDEF_LINE}"

  sed -i '1s/^.*$/'"#pragma once"'/' "include/${filename}"
  sed -i '2s/^.*$/'"${NDEF_LINE}"'/' "include/${filename}"
  sed -i '3s/^.*$/'"${DEF_LINE}"'/' "include/${filename}"
  sed -i '$ s,^.*$,'"${ENDIF_LINE}"',g' "include/${filename}"

done
