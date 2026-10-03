#!/usr/bin/env bash

cd "$(dirname "$0")"

git submodule update --init --filter=blob:none --recursive --single-branch --depth 1 --jobs $(nproc || echo 1) . || git submodule update --init --recursive --depth 1 .
