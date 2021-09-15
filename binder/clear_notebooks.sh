#!/bin/bash

################################################################################
echo
echo "running clear_notebooks.sh"
echo "---------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "other initialization"
echo "--------------------"
################################################################################

[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# adapted from https://stackoverflow.com/a/24114056
script_dir="$(dirname -- "$BASH_SOURCE")"
echo "script_dir ${script_dir}"

################################################################################
echo
echo "clear notebooks in current directory"
echo "--------------------------------------"
################################################################################

shopt -s nullglob

for notebook in "${script_dir}/"*.ipynb; do
  jupyter nbconvert \
    --ClearOutputPreprocessor.enabled=True \
    --clear-output \
    --inplace \
    "${notebook}"
  # remove empty cells
  nb-clean clean  --remove-empty-cells "${notebook}"
  # strip trailing whitespace
  sed -i 's/\s*\\n",$/\\n",/g' "${notebook}"
  sed -i 's/\s*"$/"/g' "${notebook}"
  # strip id fields
  # adapted from https://stackoverflow.com/a/68037340
  sed -i '/^ *"id": "[a-z0-9]\+",$/d' "${notebook}"

  # ensure each cell ends with exactly one empty line
  # in order for cleaner diffs
  # adapted from https://stackoverflow.com/a/1955555
  python3 -c "'';\
  import json; \
  import sys; \
  notebook = json.load(open('${notebook}', 'r')); \
  notebook['cells'] = [ \
    { \
      **cell, \
      **( \
        { \
          'source' : [ \
            f'{line}\n' \
            for line in \
            ''.join(cell['source']).rstrip().split('\n') \
          ], \
        } \
        if 'source' in cell \
        else {} \
      ), \
    } \
    for cell in notebook['cells'] \
  ]; \
  fp = open('${notebook}', 'w'); \
  json.dump( \
    notebook, \
    fp, \
    indent=1, \
  ); \
  fp.write('\n'); \
  "
done

shopt -u nullglob

################################################################################
echo
echo "recurse to subdirectories"
echo "-------------------------"
################################################################################

shopt -s nullglob

for script in "${script_dir}/"*/clear_notebooks.sh; do
  "${script}"
done

shopt -u nullglob
