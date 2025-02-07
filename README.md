# EightBallCpp

This is an enhanced C++ version of the Fortify EightBall demo application. It uses CMake for the build
process and Conan to managed open source library dependencies.

Pre-requisites:
===============

The following software is required to be installed for this project.

 - Fortify Static Code Analyzer >= 24.2 (with Fortify License)
 - Visual Studio Professional >= 2022 (if building on Windows)
 - CMake >= 3.29.6
 - Ninja >= 1.12.1
 - Python >= 3.6
 - Debricked CLI (with Debricked Enterprise account)

Setup Environment
=================

The build environment, e.g. compilers, linkers etc needs to be set before executing
any commands. On Windows you should start a command prompt: "x64 Native Tools Command Prompt for VS 2022".

Install Conan
=============

Windows:

```
python -m venv .venv
.venv\Scripts\Activate.ps1
pip install conan [--upgrade]
conan profile detect --force
conan remote update conancenter --url="https://center2.conan.io"
```

Linux:

```
python3 -m venv .venv
source .venv/bin/activate
sudo apt-get install pipx
pipx install conan
pipx ensurepath
source ~/.bashrc
conan profile detect --force
conan remote update conancenter --url="https://center2.conan.io"
```

This will create a `default` profile based on your environment, you can check it with:

```
conan profile path default
conan profile show
```

Build the Application
=====================

To build the application carry out the following:

Windows:

```
conan install . --output-folder=build --build=missing --settings=build_type=Release -c tools.cmake.cmaketoolchain:generator=Ninja
cmake --preset conan-release
cd build
.\conanbuild.bat
cmake --build . --clean-first --config Release --verbose
```

Linux/UNIX:

```
conan install . --output-folder=build --build=missing --settings=build_type=Release -c tools.cmake.cmaketoolchain:generator=Ninja
cmake --preset conan-release
cd build
source conanbuild.sh
cmake --build . --clean-first --config Release --verbose
```

Note: some of the libraries used might need to be rebuilt for your environment and so the first command might take a while.

You can check the application works by running it as follows:

Windows:

```
> .\EightBall.exe am i beautiful
You have entered a question with 3 words:
Am i beautiful?
The Magic 8 Ball says:
The outlook is poor
```

Linux/UNIX:

```
> ./EightBall am i beautiful
You have entered a question with 3 words:
Am i beautiful?
The Magic 8 Ball says:
The outlook is poor
```


Fortify SAST Scan
=================

For Fortify Static Code Analyzer to work, the `sourceanalyzer` executable needs to act as a "shim" for
each "compile" command that is executed. When using CMake and Conan this can get quite complicated as the build files
are generated. The recommended approach is for the file `compile_commands.json` to be generated via the
`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` option and used to dynamically construct the `sourceanalyzer` commands. 
Note: this option is set as the default in the `CMakeLists.txt` file. 

Note: this works on Linux/UNIX only and can be run using the example script `bin/fortify_scan.sh`:

```
../bin/fortify_scan.sh
```

On Windows an example script `bin\create_scan_file.ps1` is provided:

```
cd ..
.\bin\create_scan_file.ps1
cd build
.\fortify_scan.bat
```

If the scan is successful there will be a file called `EightBallCpp.fpr` created which you can
open with Fortify Audit Workbench:

```
auditworkbench EightBallCpp.fpr
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

Once the `mbs` file has been created using the above, the ScanCentral Command Line tool can be used to
upload it ScanCentral SAST and start the scan as in the following:

```
scancentral -url YOUR_SCANCENTRAL_URL start -uptoken 6449c23f-e287-4ef2-b3db-5e7201fb8bef -mbs .\EightBallCpp.mbs `
    -scan -build-application "EightBallCpp" -build-version "main" -block -o -f "EightBallCpp.fpr"
```

Fortify on Demand Scan
======================

Once the `mbs` file has been created using the above, the Fortify Command Line tool can be used to
upload it to Fortify on Demand and start the scan as in the following:

Windows:

```
Compress-Archive -Path .\EightBallCpp.mbs -DestinationPath fortifypackage.zip -Force
```

Linux/UNIX:

```
zip fortifypackage EightBallCpp.mbs
```

then:

```
fcli fod session login [--url YOUR_FOD_URL --client-id YOUR_CLIENT_ID --client-secret YOUR_CLIENT_SECRET]
fcli fod sast-scan start --release="EightBallCpp [KAL]:main" -f fortifypackage.zip --store curScan
fcli fod sast-scan wait-for ::curScan::
```

Debricked SCA Scan
==================

Currently Debricked does not have any native support for Conan, however Conan can create CycloneDX SBOMs and these files
can be scanned. To generate an SBOM and upload it to debricked you can carry out the following:

```
pipx install cyclonedx-conan
cyclonedx-conan conanfile.txt > sbom.json
debricked scan -r EightBallCpp -e "Lib\**" -t $Env:DEBRICKED_TOKEN
```

The SBOM uploaded to Debricked can be imported into Fortify Software Security Center using the following:

```
fcli ssc artifact import-debricked --appversion="EightBallCpp:main" --repository=EightBallCpp --branch=main -t YOUR_DEBRICKED_TOKEN
```

The SBOM can also be scanned with Fortify on Demand (Debricked Integration) using the following:

Windows:

```
Compress-Archive -Path .\sbom.json -DestinationPath fortifypackage.zip -Force
```

Linux/UNIX:

```
zip fortifypackage sbom.json
```

then:

```
Compress-Archive -Path .\sbom.json -DestinationPath fortifypackage.zip -Force
fcli fod session login [--url YOUR_FOD_URL --client-id YOUR_CLIENT_ID --client-secret YOUR_CLIENT_SECRET]
fcli fod oss-scan start --release="EightBallCpp [KAL]:main" -f fortifypackage.zip --store curScan
fcli fod oss-scan wait-for ::curScan::
```

There is also a GitHub Action [debricked.yml](.github/workflows/debricked.yml) included to carry this out automatically.

---

Kevin Lee - klee2@opentext.com

