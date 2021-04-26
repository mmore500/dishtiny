#!/bin/sh

apt-get update -qq \
  && apt-get install -qq --no-install-recommends \
    libmetis-dev \
    metis \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  || pacman -S metis \
  || yum install metis \
  || echo "package installer not supported! please install METIS manually"
