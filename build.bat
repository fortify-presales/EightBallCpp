@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build

conan install . --output-folder=build --build=missing --settings=build_type=Debug
PUSHD build
cmake .. -G "Visual Studio 17 2022" -D"CMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake"
cmake --build . --config Debug --verbose
POPD

POPD
