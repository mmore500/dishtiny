#!/bin/bash

VERSION=$(git describe --dirty --always)
TIME=$(date +%s)

OSF_PATH="documentation-coverage/time=${TIME}+version=${VERSION}"

set -o xtrace

#TODO
# osf -u "$OSF_USERNAME" -p 7jkgp upload _build/doc-coverage.json "${OSF_PATH}/doc-coverage.json"
