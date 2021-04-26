#!/bin/bash

# After success travis code
git config --global user.email 'travis@travis-ci.org'
git config --global user.name 'Travis CI'
make web
make badges

cd $(mktemp -d)

git clone https://${GH_TOKEN}@github.com/mmore500/dishtiny.git -b web-archive
mkdir -p dishtiny/master
cp /opt/dishtiny/web/* dishtiny/master

cd dishtiny
git add .
git commit --message "Travis build ${TRAVIS_BUILD_NUMBER}"

git checkout -b gh-pages

git push --force --quiet origin gh-pages
