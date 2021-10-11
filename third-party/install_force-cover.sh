#!/bin/bash

git submodule update --init --recursive

make -C force-cover/
