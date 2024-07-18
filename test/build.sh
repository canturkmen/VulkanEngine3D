#!/bin/bash
# Build script for test
set echo on

mkdir -p ../bin

# Get a list of all the .c files
cFileNames = $(find . -type f -name "*.c")

#echo "Files:" $cFileNames

assembly="test"
compilerFlags="-g -fdeclspec -fPIC"
# -fms-extensions
# -Wall -Werror
includeFlags="-Isrc -I../engine/src/"
linkerFlags="-L../bin/ -lengine -Wl, -rpath,."
defines="-D_DEBUG -DKIMPORT"

echo "Building $assembly"
echo clang $cFileNames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
clang $cFileNames $compilerFlags -o ../bin/$assembly $defines $includeFlags $linkerFlags
