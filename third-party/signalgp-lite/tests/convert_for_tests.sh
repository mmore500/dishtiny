#!/usr/bin/env bash

TO_ROOT=$(git rev-parse --show-cdup)

set -e # exit with error if any of this fails
rm -rf ${TO_ROOT}/coverage_include
cp -r ${TO_ROOT}/include ${TO_ROOT}/coverage_include
for filename in $(find ${TO_ROOT}/coverage_include -name "*.hpp")
do
    echo $filename
    ${TO_ROOT}/third-party/force-cover/force_cover $filename -- -I. -I${TO_ROOT}/coverage_include --language c++ -std=c++17 -I/usr/bin/../lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8 -I/usr/bin/../lib/gcc/x86_64-linux-gnu/8/../../../../include/x86_64-linux-gnu/c++/8 -I/usr/bin/../lib/gcc/x86_64-linux-gnu/8/../../../../include/c++/8/backward -I/usr/include/clang/7/include -I/usr/local/include -I/usr/bin/../lib/gcc/x86_64-linux-gnu/8/include -I/usr/include/x86_64-linux-gnu -I/usr/include -I${TO_ROOT}/third-party/cereal/include/ | xargs -0 echo > $filename.temp
    mv $filename.temp $filename

done
