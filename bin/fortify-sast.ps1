#
# Example script to perform Fortify Static Code Analysis
#

# Parameters
param (
    [Parameter(Mandatory=$false)]
    [ValidateSet('classic','security','devops')]
    [string]$ScanPolicy = "classic",
    [Parameter(Mandatory=$false)]
    [switch]$SkipPDF,
    [Parameter(Mandatory=$false)]
    [switch]$SkipSSC,
    [Parameter(Mandatory=$false)]
    [switch]$DontRun
)

# Import some supporting functions
Import-Module $PSScriptRoot\modules\FortifyFunctions.psm1

# Import local environment specific settings
$EnvSettings = $(ConvertFrom-StringData -StringData (Get-Content ".\.env" | Where-Object {-not ($_.StartsWith('#'))} | Out-String))
$AppName = $EnvSettings['SSC_APP_NAME']
$AppVersion = $EnvSettings['SSC_APP_VER_NAME']
$SSCUrl = $EnvSettings['SSC_URL']
$SSCAuthToken = $EnvSettings['SSC_AUTH_TOKEN'] # CIToken
$JVMArgs = "-Xss256M"
$ScanSwitches = ""
$FortifyScanCommands = "fortify_scan.bat"

# Test we have Fortify installed successfully
Test-Environment
if ([string]::IsNullOrEmpty($AppName)) { throw "Application Name has not been set" }

# Run the translation and scan

# Check if compile_commands.json exists in build directory
# TODO: check base location
$BuildDir = Join-Path -Path (Get-Location) -ChildPath "build"
if (-not (Test-Path $CompileCommandsFile)) {
    throw "The build directory does not exist in the build directory; has the build been run?"
}
$CompileCommandsFile = Join-Path -Path $BuildDir -ChildPath "compile_commands.json"
if (-not (Test-Path $CompileCommandsFile)) {
    throw "The compile_commands.json file does not exist in the build directory; has the build been run?"
}

Write-Host Running translation...
Push-Location -Path $BuildDir
$CompileCommands = Get-Content $CompileCommandsFile -Raw | ConvertFrom-Json 
foreach ($command in $CompileCommands.command) 
{
    $cl = $command.Replace('-external:I', "/I")
    "sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose $cl" 
}

Write-Host Running scan...
sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' $JVMArgs $ScanSwitches -b "$AppName" -debug -verbose `
    -rules ../etc/sast-custom-rules/example-custom-rules.xml -filter ../etc/sast-filters/example-filter.txt `
    -scan-policy $ScanPolicy `
    -build-project "$AppName" -build-version "$AppVersion" -build-label "SNAPSHOT" `
    -scan -f "$($AppName).fpr"

fprutility -information -analyzerIssueCounts -project "$($AppName).fpr" 

if (-not $SkipPDF) {
    ReportGenerator '-Dcom.fortify.sca.ProjectRoot=.fortify' -user "Demo User" -format pdf -f "$($AppName).pdf" -source "$($AppName).fpr"
}

if (-not $SkipSSC) {
    fortifyclient uploadFPR -file "$($AppName).fpr" -url $SSCUrl -authtoken $SSCAuthToken -application $AppName -applicationVersion $AppVersion
}

Write-Host "Done."

Pop-Location
