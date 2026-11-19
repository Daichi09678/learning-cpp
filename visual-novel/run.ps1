param([switch]$Rebuild, [string]$MsysBin = 'C:\msys64\ucrt64\bin')

$ErrorActionPreference = 'Stop'
$exe = Join-Path $PSScriptRoot 'out\windows\mercusuar.exe'
if ($Rebuild -or -not (Test-Path -LiteralPath $exe -PathType Leaf)) {
    & (Join-Path $PSScriptRoot 'build.ps1') -MsysBin $MsysBin
}
& $exe
if ($LASTEXITCODE -ne 0) { throw "Demo berhenti dengan kode $LASTEXITCODE." }
