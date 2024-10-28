#!/bin/bash
echo "Executing run script for build job"
error=0

echo "Build the cmake component for x86"
mkdir -p build/build_x86 && cd build/build_x86
cmake ../..
make -j 8

if [ $? -ne 0 ]; then
        echo "build returns ERROR."
        error=$((error+1))
else
        echo "build returns OK."
fi

cd ../../