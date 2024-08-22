REM
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files.
SET cFileNames=
FOR /R %%f in (*.c) do (SET cFileNames=!cFileNames! %%f)

REM echo "Files:" %cFileNames%

SET assembly=tests
SET compilerFlags=-g -Wno-missing-braces
REM -Wall -Werror -save-temps=obj -O0
SET includeFlags=-Isrc -I../engine/src
SET linkerFlags=-L../bin/ -lengline.lib
SET defines=-D_DEBUG -DVEIMPORT

ECHO "Building %assembly%%..."
clang %cFileNames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%