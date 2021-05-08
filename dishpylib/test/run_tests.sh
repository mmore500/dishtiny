#!/bin/bash

set -e # exit with error if any of this fails

script_dir="$(dirname "$(readlink -f "$0")")"

nosetests "${script_dir}"/**/test_*.py
