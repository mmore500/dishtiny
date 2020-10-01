#!/bin/bash

git submodule update --init --recursive

cd force-cover && make
