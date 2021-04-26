#!/bin/sh

cd Catch
cmake -Bbuild -H. -DBUILD_TESTING=OFF
cmake --build build/ --target install
