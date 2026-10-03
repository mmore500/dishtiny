#!/usr/bin/env bash

cd "$(dirname "$0")"

./submodules.sh
./install_emsdk.sh
./install_force-cover.sh
