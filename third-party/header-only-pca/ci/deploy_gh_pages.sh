#!/bin/bash

# After success travis code
git config --global user.email 'travis@travis-ci.org'
git config --global user.name 'Travis CI'
make badges
cd web
git init
git remote add origin https://${GH_TOKEN}@github.com/rodsan0/header-only-pca.git > /dev/null 2>&1
git checkout -b gh-pages
git add .
git commit --message "Travis build ${TRAVIS_BUILD_NUMBER}"
git push --force --quiet origin gh-pages || echo "didn't push, not on deployment branch"
