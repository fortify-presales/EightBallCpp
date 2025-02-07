#
# Example script to create a file containing Fortify Static Code Analyzer commands
#

# Parameters
param (
    [Parameter(Mandatory=$false)]
    [ValidateSet('classic','security','devops')]
    [string]$ScanPolicy = "classic"
)

# Import some supporting functions
Import-Module $PSScriptRoot\modules\FortifyFunctions.psm1

# Import local environment specific settings
$EnvSettings = $(ConvertFrom-StringData -StringData (Get-Content ".\.env" | Where-Object {-not ($_.StartsWith('#'))} | Out-String))
$AppName = $EnvSettings['APP_NAME']
$AppVersion = $EnvSettings['APP_VER_NAME']
$JVMArgs = "-Xss256M"
$ScanSwitches = ""
$FortifyScanFile = "fortify_scan.bat"

# Test we have Fortify installed successfully
Test-Environment
if ([string]::IsNullOrEmpty($AppName)) { $AppName="EightBallCpp" }
if ([string]::IsNullOrEmpty($AppVersion)) { $AppVersion="1.0" }

# Run the translation and scan

# Check if compile_commands.json exists in build directory
# TODO: check base location
$BuildDir = Join-Path -Path (Get-Location) -ChildPath "build"
if (-not (Test-Path $BuildDir)) {
    throw "The build directory does not exist in the build directory; has the build been run?"
}
$CompileCommandsFile = Join-Path -Path $BuildDir -ChildPath "compile_commands.json"
if (-not (Test-Path $CompileCommandsFile)) {
    throw "The compile_commands.json file does not exist in the build directory; has the build been run?"
}

Write-Host "Creating $FortifyScanFile in $BuildDir"

Push-Location -Path $BuildDir
if (Test-Path $FortifyScanFile) {
    Remove-Item -Force $FortifyScanFile
}
"@ECHO ON" | Out-File -Encoding UTF8 $FortifyScanFile    

# Create Translation commands
$CompileCommands = Get-Content $CompileCommandsFile -Raw | ConvertFrom-Json 
foreach ($command in $CompileCommands.command) 
{
    $cl = $command.Replace('-external:I', "/I")
    "sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose $cl" `
        | Out-File -Encoding UTF8 -Append $FortifyScanFile
}

# Create MBS file in case we need it
"sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify -Dcom.fortify.sca.MobileBuildSessions=true $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose -export-build-session ""$($AppName).mbs""" `
    | Out-File -Encoding UTF8 -Append $FortifyScanFile

# Create Scan command    
"sourceanalyzer -Dcom.fortify.sca.ProjectRoot=.fortify $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose ^`
    -rules ../etc/sast-custom-rules/example-custom-rules.xml -filter ../etc/sast-filters/example-filter.txt ^`
    -scan-policy $ScanPolicy ^`
    -build-project ""$AppName"" -build-version ""$AppVersion"" -build-label ""SNAPSHOT"" ^`
    -scan -f ""$($AppName).fpr"""`
        | Out-File -Encoding UTF8 -Append $FortifyScanFile

# Add scan summary command
"fprutility -information -analyzerIssueCounts -project ""$($AppName).fpr""" `
    | Out-File -Encoding UTF8 -Append $FortifyScanFile

Pop-Location

Write-Host "Done."
