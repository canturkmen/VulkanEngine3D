#!/bin/bash
# Build script for engine
set echo on

mkdir -p ../bin

# Get a list of all the .c files
cFileNames = $(find . -type f -name "*.c")

#echo "Files:" $cFileNames

assembly="engine"
compilerFlags="-g -shared -fdeclspec -fPIC"
# -fms-extensions
# -Wall -Werror
includeFlags="-Isrc -I$VULKAN_SDK/include"
linkerFlags="-lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$VULKAN_SDK/lib -L/usr/X11R6/lib"
defines="-D_DEBUG -DKEXPORT"

echo "Building $assembly"
clang $cFileNames $compilerFlags -o ../bin/lib$assembly.so $defines $includeFlags $linkerFlags