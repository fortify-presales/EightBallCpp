# EightBallCpp

This is an enhanced C++ version of the Fortify EightBall demo application. It uses CMake for the build
process and Conan to managed open source library dependencies.

Pre-requisites:
===============

The following software is required to be installed for this project.

 - Fortify Static Code Analyzer 24.2 or later
 - Debricked CLI (and Debricked Enterprise account)
 - Visual Studio Professional 2022 or later (for Windows build)
 - CMake >= 3.15
 - Python >= 3.6
 
Install Conan
=============

```
python -m venv .
pip install conan
conan profile detect --force
```

This will create a `default` profile based on your environment, you can check it with:

```
conan profile path default
conan profile show
```

Note: the default profile will set to a `Release` build but we will be overriding with `Debug` in our examples.

Build the Application
=====================

To build the application carry out the following:

Windows:

```
> clean.bat
> build.bat
```

Linux/UNIX:

```
$ clean.sh
$ build.sh
```

You can check the application works by running it as follows:

```
> .\build\Debug\EightBall.exe am i beautiful
You have entered a question with 3 words:
Am i beautiful?
The Magic 8 Ball says:
The outlook is poor
```


Fortify SAST Scan
=================

For Fortify Static Code Analyzer to work, the `sourceanalyzer` executable needs to act as a "shim" for
each "compile" command. When using CMake and Conan this can get quite complicated as the build files
are generated. Since on Windows MSBuild is used it is possible to simply wrap the `msbuild` build command
with `sourceananalyzer`. However, you need to make sure that all of the appropriate Visual Studio
environment variables are set. To do this make sure you are running the following commands from a Visual Studio
Developer's Command prompt (or use the `setvsenv.ps` script to set them).

Windows:

```
scan.bat
```

Linux/UNIX:

```
TBD
```

If the scan is successful there will be a file called `EightBallCpp.fpr` created which you can
open with Fortify Audit Workbench:

```
> auditworkbench EightBallCpp.fpr
```

Fortify ScanCentral SAST Scan
=============================

TBD

Fortify on Demand Scan
======================

TBD

Debricked SCA Scan
==================

Currently Debricked does not have any native support for Conan, however Conan can create
CycloneDX SBOMs as in the following:

```
pip install cyclonedx-conan
cyclonedx-conan .\conanfile.txt > sbom.json
debricked scan -r EightBallCpp -t YOUR_DEBRICKED_ACCESS_TOKEN
```
---

Kevin Lee - klee2@opentext.com

