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
$AppName = $EnvSettings['SSC_APP_NAME']
$AppVersion = $EnvSettings['SSC_APP_VER_NAME']
$SSCUrl = $EnvSettings['SSC_URL']
$SSCAuthToken = $EnvSettings['SSC_AUTH_TOKEN'] # CIToken
$JVMArgs = "-Xss256M"
$ScanSwitches = ""
$FortifyScanFile = "fortify_scan.bat"

# Test we have Fortify installed successfully
Test-Environment
if ([string]::IsNullOrEmpty($AppName)) { throw "Application Name has not been set" }

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
if ($FortifyScanFile) {
    Remove-Item -Force $FortifyScanFile
}
"@ECHO ON" | Out-File -Encoding UTF8 -Append $FortifyScanFile    

# Create Translation commands
$CompileCommands = Get-Content $CompileCommandsFile -Raw | ConvertFrom-Json 
foreach ($command in $CompileCommands.command) 
{
    $cl = $command.Replace('-external:I', "/I")
    "sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose $cl" `
        | Out-File -Encoding UTF8 -Append $FortifyScanFile
}

# Create MBS file in case we need it
"sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' -b ""$AppName"" -debug -verbose ^`
    '-Dcom.fortify.sca.MobileBuildSessions=true' -export-build-session ""$($AppName).mbs""" `
    | Out-File -Encoding UTF8 -Append $FortifyScanFile

# Create Scan command    
"sourceanalyzer '-Dcom.fortify.sca.ProjectRoot=.fortify' $JVMArgs $ScanSwitches -b ""$AppName"" -debug -verbose ^`
    -rules ../etc/sast-custom-rules/example-custom-rules.xml -filter ../etc/sast-filters/example-filter.txt ^`
    -scan-policy $ScanPolicy ^`
    -build-project ""$AppName"" -build-version ""$AppVersion"" -build-label ""SNAPSHOT"" ^`
    -scan -f ""$($AppName).fpr"""`
        | Out-File -Encoding UTF8 -Append $FortifyScanFile

# Add scan summary command
"fprutility -information -analyzerIssueCounts -project ""$($AppName).fpr""" `
    | Out-File -Encoding UTF8 -Append $FortifyScanFile

"PAUSE" | Out-File -Encoding UTF8 -Append $FortifyScanFile    

Pop-Location

Write-Host "Done."
