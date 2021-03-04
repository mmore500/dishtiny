#!/bin/bash

# fail on error
set -e

git config --global user.email '41898282+github-actions[bot]@users.noreply.github.com'
git config --global user.name 'github-actions'
git config --global init.defaultBranch 'master'

for folder in $(realpath config/*/*/*); do

  cd "${folder}"

  rm -rf .git

  revision_tag="a=$(git rev-parse --show-prefix)+revision=${GITHUB_SHA}"
  ref_tag="a=$(git rev-parse --show-prefix)+ref=${GITHUB_REF}"

  git init
  git remote add origin "https://mmore500:${PERSONAL_ACCESS_TOKEN}@github.com/mmore500/dishtiny-assets.git"

  git -c commit.gpgsign=false commit --allow-empty --message "Initial commit"
  git checkout --quiet $(git rev-parse HEAD)

  # allows empty tar file
  printf '' | tar -czf assets.tar.gz --files-from -
  git add assets.tar.gz

  git -c commit.gpgsign=false commit --message "GITHUB ACTION"

  echo "revision_tag ${revision_tag}"
  git tag "${revision_tag}"
  git push --force origin "${revision_tag}"

  echo "ref_tag ${ref_tag}"
  git tag "${ref_tag}"
  git push --force origin "${ref_tag}"

  rm -rf .git assets.tar.gz

done
