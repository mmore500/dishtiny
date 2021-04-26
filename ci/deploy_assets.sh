#!/bin/bash

# fail on error
set -e

git config --global user.email '41898282+github-actions[bot]@users.noreply.github.com'
git config --global user.name 'github-actions'
git config --global init.defaultBranch 'master'

for folder in $(realpath configpacks/*/*); do

  # skip non-directories
  test -d "${folder}" || continue

  cd "${folder}"

  rm -rf .git assets.tar.gz

  revision_tag="$(git rev-parse --show-prefix)@${GITHUB_SHA}"
  ref_tag="$(git rev-parse --show-prefix)@${GITHUB_REF}"

  git init
  git remote add origin "https://mmore500:${PERSONAL_ACCESS_TOKEN}@github.com/mmore500/dishtiny-assets.git"

  git -c commit.gpgsign=false commit --allow-empty --message "Initial commit"
  git checkout --quiet $(git rev-parse HEAD)

  # allows empty tar file
  ls | tar -czvf assets.tar.gz --files-from -
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
