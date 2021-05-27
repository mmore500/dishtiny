#!/bin/bash

DIRECTORY_EXCLUDES=$(for submodule in $(ls -d third-party/*/); do printf " --exclude-dir=`basename $submodule`"; done)

grep -iIER $DIRECTORY_EXCLUDES --exclude-dir=_build --exclude-dir=api --exclude-dir=doxyoutput --exclude="*dotos*" 'todo|fixme' . | wc -l
