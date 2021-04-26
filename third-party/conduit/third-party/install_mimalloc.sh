#!/bin/sh

git clone --recursive https://github.com/microsoft/mimalloc
cd mimalloc
git reset --hard 13a4030619edada133e1a3c06c6287ad5a9c74c1
mkdir -p out/release
cd out/release
cmake ../..
make install
