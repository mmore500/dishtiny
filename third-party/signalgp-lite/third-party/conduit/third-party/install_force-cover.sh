#!/bin/bash

git clone --recursive https://github.com/emilydolson/force-cover
cd force-cover
git reset --hard fa52e3177812320ddce39dc11223981c5dee958c
make
