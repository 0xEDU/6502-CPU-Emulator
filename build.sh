#!/bin/bash

DIR=build

mkdir -p $DIR
echo -e "Building..."
cmake --build $DIR
echo -e "Running make..."
make --no-print-dir -C $DIR -j4
