#!/bin/bash

AppName="EightBallCpp"

echo "Removing files..."
rm -rf build
rm -f "CMakePresets.json"
rm -f "CMakeUserPresets.json"
rm -rf ".fortify"
rm -f "${AppName}.fpr"
rm -f "${AppName}.pdf"
rm -f "*Package.zip"
rm -f "sbom.*"

echo "Done."
