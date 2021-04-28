#!/bin/bash

git clone --recursive https://github.com/emscripten-core/emsdk
cd emsdk
git reset --hard e88a3c5bbfef172a5b947768204ef734e2fb6e04

# Download and install pinned SDK tools.
./emsdk install 1.38.48

# Make the pinned SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate 1.38.48

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
cd ..
