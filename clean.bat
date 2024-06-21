@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S .fortify
RMDIR /Q /S build

POPD
