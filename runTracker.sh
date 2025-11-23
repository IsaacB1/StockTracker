#!/bin/sh

cd build/

echo "Moving to build"

rm -rf * 

echo "deleting old build files"

cmake ..
make

cd ..

./build/isaacStocks