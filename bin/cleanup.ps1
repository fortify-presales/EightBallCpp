
# Import some supporting functions
Import-Module $PSScriptRoot\modules\FortifyFunctions.psm1

# Import local environment specific settings
$EnvSettings = $(ConvertFrom-StringData -StringData (Get-Content ".\.env" | Where-Object {-not ($_.StartsWith('#'))} | Out-String))
$AppName = $EnvSettings['SSC_APP_NAME']

Write-Host "Removing files..."
Remove-Item -Force -Recurse "build" -ErrorAction SilentlyContinue
Remove-Item "CMakePresets.json" -ErrorAction SilentlyContinue
Remove-Item "CMakeUserPresets.json" -ErrorAction SilentlyContinue
Remove-Item -Force -Recurse ".fortify" -ErrorAction SilentlyContinue
Remove-Item "$($AppName)*.fpr" -ErrorAction SilentlyContinue
Remove-Item "$($AppName)*.pdf" -ErrorAction SilentlyContinue
Remove-Item "iqReport.json" -ErrorAction SilentlyContinue
Remove-Item "fod.zip" -ErrorAction SilentlyContinue
Remove-Item "*Package.zip" -ErrorAction SilentlyContinue
Remove-Item "sbom.*" -ErrorAction SilentlyContinue

Write-Host "Done."
