#!/bin/bash

Help()
{
    # Display Help
    echo "Fortify SAST scan helper script."
    echo
    echo "Syntax: fortify_scan.sh [p|t|h|v|V]"
    echo "options:"
    echo "p     set the scan policy (devops, security, classic)"
    echo "t     Translate only - no Scan"
    echo "h     Print this Help."
    echo "v     Verbose mode."
    echo
}


AppName="EightBallCpp"
AppVersion="1.0"
DoScan=true
JVMArgs="-Xss256M"
ScanSwitches=""


while getopts ":hp:" option; do
    case $option in
        h) # display Help
            Help
            exit;;
        p) # Enter a scan policy
            Policy=$OPTARG;;
        t) # Translation only
            DoScan=false;;
        \?) # Invalid option
            echo "Error: Invalid option"
            exit;;
    esac
done

if [[ "$Policy" -eq "" ]]; then
    Policy="classic"
fi
echo "Using scan policy $Policy"
echo "Running translation ..."
sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify $JVMArgs -b "$AppName" -debug -verbose compile_commands.json
echo "Creating mobile build session"
sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify -Dcom.fortify.sca.MobileBuildSessions=true $JVMArgs \
    -b "$AppName" -debug -verbose -export-build-session "${AppName}.mbs" 
if [ "$DoScan" == true ]; then
    echo "Running scan ..."
    sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify $JVMArgs -b "$AppName" -debug -verbose \
        -rules ../etc/sast-custom-rules/example-custom-rules.xml -filter ../etc/sast-filters/example-filter.txt \
        -scan-policy $Policy \
        -build-project "$AppName" -build-version "$AppVersion" -build-label "SNAPSHOT" \
        -scan -f "${AppName}.fpr"
    FPRUtility -information -analyzerIssueCounts -project "${AppName}.fpr"
fi
