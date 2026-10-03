#!/usr/bin/env bash

cd "$(dirname "$0")"

./submodules.sh

make -C force-cover/
