#!/bin/bash

DIR=build

mkdir -p $DIR
echo -e "Building..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B $DIR
cp $DIR/compile_commands.json .
echo -e "Running make..."
make --no-print-dir -C $DIR -j4
