#!/bin/sh

# adapted from https://stackoverflow.com/a/43136160
mkdir benchmark && cd benchmark
git init
git remote add origin https://github.com/google/benchmark
git fetch --depth 1 origin 37177a84b7e8d33696ea1e1854513cb0de3b4dc3
git checkout FETCH_HEAD

# adapted from https://stackoverflow.com/a/43136160
mkdir googletest && cd googletest
git init
git remote add origin https://github.com/google/googletest
git fetch --depth 1 origin c4a5ee3ac0d4a457fa50b7381f603b6aea1e6897
git checkout FETCH_HEAD
cd ..

# verify checked-out commit
git rev-parse HEAD

mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j8
make install

# verify checked-out commit
git rev-parse HEAD

# apt-get update -qq \
#   && apt-get install -qq --no-install-recommends \
#     libbenchmark1 \
#     libbenchmark-dev \
#   && apt-get clean \
#   && rm -rf /var/lib/apt/lists/* \
#   || echo "package installer not supported!" \
#     && echo "please install Google Benchmark manually"
