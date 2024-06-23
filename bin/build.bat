@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

RMDIR /Q /S build

conan install . --output-folder=build --build=missing --settings=build_type=Release -c tools.cmake.cmaketoolchain:generator=Ninja
REM conan install . --output-folder=build --build=missing --settings=build_type=Debug
PUSHD build
.\conanbuild.bat
REM cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=%BASEDIR%build\conan_toolchain.cmake -D'CMAKE_EXPORT_COMPILE_COMMANDS=ON'
cmake .. -G Ninja -D'CMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake' -D'CMAKE_EXPORT_COMPILE_COMMANDS=ON'
REM # -DCMAKE_EXPORT_COMPILE_COMMANDS=yes
cmake --build . --clean-first --config Release --verbose
POPD

POPD
