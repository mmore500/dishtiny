#!/bin/bash

git clone --recursive https://github.com/emscripten-core/emsdk
cd emsdk
git reset --hard e88a3c5bbfef172a5b947768204ef734e2fb6e04
./emsdk install 1.38.48
./emsdk activate 1.38.48
source ./emsdk_env.sh
