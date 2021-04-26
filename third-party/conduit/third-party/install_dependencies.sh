#!/bin/bash

git submodule update --init --recursive

./install_cmake.sh
./install_catch2.sh
./install_benchmark.sh
./install_emsdk.sh
./install_force-cover.sh
./install_mimalloc.sh
./install_metis.sh
