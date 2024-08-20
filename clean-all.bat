@ECHO OFF
REM Clean everything

ECHO "Cleaning everything..."

REM Engine
make -f "Makefile.engine.windows.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Test
make -f "Makefile.test.windows.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies cleaned succesfully."