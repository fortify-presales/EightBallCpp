@ECHO ON

set BASEDIR=%~dp0
PUSHD %BASEDIR%

set APP_NAME="EightBallCpp"
set APP_VERSION="latest"
set SCAN_POLICY="security"
set JVM_ARGS=""
set SCAN_SWITCHES=""

PUSHD build
sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' %JVM_ARGS% %SCAN_SWITCHES% -b %APP_NAME% ^
 -debug -verbose msbuild ALL_BUILD.vcxproj /p:Configuration=Debug
POPD

sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' %JVM_ARGS% %SCAN_SWITCHES% -b %APP_NAME% ^
    -rules etc\sast-custom-rules\example-custom-rules.xml -filter etc\sast-filters\example-filter.txt ^
    -scan-policy %SCAN_POLICY% ^
    -build-project %APP_NAME% -build-version %APP_VERSION% -build-label "SNAPSHOT" ^
    -scan -f %APP_NAME%.fpr

POPD
