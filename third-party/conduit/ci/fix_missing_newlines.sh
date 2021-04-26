#!/bin/bash

TARGETS=$(find . -type f ! -path "./third-party/*" ! -path "./.git/*" ! -path "*/assets/*" ! -path "*.tar.gz" ! -path "*.jpg" ! -path "*.png" ! -path "*Makefile" ! -path "*Maketemplate*" ! -path "./node_modules/*")

for filename in $TARGETS
do
  sed -i -e '$a\' $filename
done
