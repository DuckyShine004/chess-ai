#!/bin/bash

mkdir -p build

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
# cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

cmake --build build -j

rm -rf .cache/logs/*
