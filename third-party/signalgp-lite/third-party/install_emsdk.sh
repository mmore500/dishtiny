#!/bin/bash

# Get the emsdk repo
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory
cd emsdk

# Fetch the latest version of the emsdk (not needed the first time you clone)
git pull

# Download and install pinned SDK tools.
./emsdk install 1.38.48

# Make the pinned SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate 1.38.48

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
cd ..
