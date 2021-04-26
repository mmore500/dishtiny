#!/bin/bash

git config --global user.email "travis@travis-ci.org"
git config --global user.name "Travis CI"
cd web
git init
git remote add origin https://${GH_TOKEN}@github.com/mmore500/conduit.git > /dev/null 2>&1
git checkout -b gh-pages
git add .
git commit --message "Travis build ${TRAVIS_BUILD_NUMBER}"
git push --force --quiet origin gh-pages
