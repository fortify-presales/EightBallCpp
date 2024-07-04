# EightBallCpp

This is an enhanced C++ version of the Fortify EightBall demo application. It uses CMake/Ninja for the build
process and Conan to managed open source library dependencies.

Pre-requisites:
===============

The following software is required to be installed for this project.

 - [Fortify Static Code Analyzer](https://www.opentext.com/en-gb/products/fortify-static-code-analyzer) >= 24.2 (with Fortify License)
 - [Visual Studio Professional](https://visualstudio.microsoft.com/vs/) >= 2022 (if building on Windows)
 - [GCC C++ Compiler](https://gcc.gnu.org/) >= 13.2.0 (if building on Linux/UNIX)
 - [CMake](https://cmake.org/download/) >= 3.29.6
 - [Ninja build system](https://ninja-build.org/) >= 1.12.1
 - [Python](https://www.python.org/downloads/) >= 3.6
 - [Debricked CLI](https://docs.debricked.com/tools-and-integrations/cli/debricked-cli) (with Debricked Enterprise account)

Setup Environment
=================

The build environment, e.g. compilers, linkers etc needs to be set before executing any commands. 
On Windows you should start-up and run all commands in Visual Studio x64 Native Tools command prompt. From the Windows menu search
for "x64 Native Tools Command Prompt for VS 2022".

Install Conan
=============

```
python -m venv venv
.\venv\Scripts\Activate.ps1 [on Windows]
source ./venv/bin/activate [on Linux/UNIX]
pip install conan [--upgrade]
conan profile detect --force
```

This will create a `default` profile based on your environment, you can check it with:

```
conan profile path default
conan profile show
```

Build the Application
=====================

To build the application carry out the following:

```
conan install . --output-folder=build --build=missing --settings=build_type=Release -c tools.cmake.cmaketoolchain:generator=Ninja
cmake --preset conan-release
cd build
.\conanbuild.bat [Windows]
sh ./conanbuild.sh [Linux/UNIX]
cmake --build . --clean-first --config Release --verbose
```

Note: some of the libraries used might need to be rebuilt for your environment and so the first command might take a while.

You can check the application works by running it as follows:

```
> .\EightBall.exe will i win the lottery [Windows]
> ./EightBall will i win the lottery [Linux/UNIX]
MAGIC 8 BALL VERSION:1.0
------------------------
You have entered a question with 5 words: Will i win the lottery?
The Magic 8 Ball says: You may rely on it.
```

Fortify SAST Scan
=================

For Fortify Static Code Analyzer to work, the `sourceanalyzer` executable needs to act as a "shim" for
each "compile" command that is executed. When using CMake the recommended approach is for the file
`compile_commands.json` to be generated via the `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` option and fed
into `sourceanalyzer` for translation. For example:

```
sourceanalyzer -b <build_id> compile_commands.json
```

However, this is only supported on Linux/UNIX. On Windows we will use the same file but dynamically construct 
the `sourceanalyzer` commands into a script. 

**Note: the `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` option is set as the default in the `CMakeLists.txt` file.**

For Linux/UNIX an example script [fortify_scan.sh](bin\fortify_scan.sh) is provided. To run the Fortify SAST scan locally:

```
[from the build directory]
..\bin\fortify_scan.sh
```

On Windows, an example script [create_scan_file.ps1](bin\create_scan_file.ps1) is provided to create the `fortify_scan.bat`
file. To run the Fortify SAST scan locally:

```
[from the root directory]
.\bin\create_scan_file.ps1
cd build
.\fortify_scan.bat
```

If the scan is successful there will be a file called `EightBallCpp.fpr` created which you can open with Fortify Audit Workbench:

```
.\auditworkbench EightBallCpp.fpr
```

Uploading Results to Fortify Software Security Center
=====================================================

Once the `fpr` file has been created using the above, the Fortify Command Line tool can be used to
upload it SSC as in the following:

```
fcli ssc session login [--url YOUR_SSC_URL --ci-token=YOUR_SSC_CI_TOKEN]
fcli ssc artifact upload --appversion="EightBallCpp:main" -f .\EightBallCpp.fpr --store curUpload
fcli ssc artifact wait-for ::curUpload::
```

Fortify ScanCentral SAST Scan
=============================

The scripts above will also create a Mobile Build Session `mbs` file which can be used with the ScanCentral Command Line tool 
to start a remote scan:

```
scancentral -url YOUR_SCANCENTRAL_URL start -uptoken 6449c23f-e287-4ef2-b3db-5e7201fb8bef -mbs .\EightBallCpp.mbs `
    -scan -build-application "EightBallCpp" -build-version "main" -block -o -f "EightBallCpp.fpr"
```

Fortify on Demand Scan
======================

Once the `mbs` file has been created using the above, the Fortify Command Line tool can be used to
upload it to Fortify on Demand and start the scan as in the following:

```
Compress-Archive -Path .\EightBallCpp.mbs -DestinationPath FoDPackage.zip -Force
fcli fod session login [--url YOUR_FOD_URL --client-id YOUR_CLIENT_ID --client-secret YOUR_CLIENT_SECRET]
fcli fod sast-scan start --release="EightBallCpp [KAL]:main" -f FoDPackage.zip --store curScan
fcli fod sast-scan wait-for ::curScan::
```

Debricked SCA Scan
==================

Currently Debricked does not have any native support for Conan, however Conan can create CycloneDX SBOMs and these files
can be scanned. To generate an SBOM and upload it to debricked you can carry out the following:

Conan 1.4.x

```
python -m venv .
.\Scripts\Activate.ps1
pip install cyclonedx-conan
cyclonedx-conan .\conanfile.txt > sbom.json
debricked scan -r EightBallCpp -e "Lib\**" -t $Env:DEBRICKED_TOKEN
```

Conan 2.x

```
python -m venv . 
.\Scripts\Activate.ps1
conan sbom:cyclonedx .\conanfile.txt -f 1.4_xml --name EightBallCpp --version 1.0. > bom.xml
```

The SBOM uploaded to Debricked can be imported into Fortify Software Security Center using the following:

```
fcli ssc artifact import-debricked --appversion="EightBallCpp:main" --repository=EightBallCpp --branch=main -t YOUR_DEBRICKED_TOKEN
```

The SBOM can also be scanned with Fortify on Demand (Debricked Integration) using the following:

```
Compress-Archive -Path .\bom.xml -DestinationPath FoDPackage.zip -Force
fcli fod session login [--url YOUR_FOD_URL --client-id YOUR_CLIENT_ID --client-secret YOUR_CLIENT_SECRET]
fcli fod oss-scan start --release="EightBallCpp [KAL]:main" -f FoDPackage.zip --store curScan
fcli fod oss-scan wait-for ::curScan::
```

There is also a GitHub Action [debricked.yml](.github/workflows/debricked.yml) included to carry this out automatically.

---

Kevin Lee - klee2@opentext.com

