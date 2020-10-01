#!/bin/bash

git submodule update --init --recursive

./install_emsdk.sh
./install_force-cover.sh
