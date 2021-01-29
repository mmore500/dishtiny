#!/bin/bash

# Get the emsdk repo
git submodule update --init --recursive

# Enter that directory
cd emsdk

# Fetch the latest version of the emsdk (not needed the first time you clone)
git pull

# Download and install the latest SDK tools.
./emsdk install 2.0.5

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate 2.0.5

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
cd ..
