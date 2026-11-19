param([string]$MsysBin = 'C:\msys64\ucrt64\bin')

$ErrorActionPreference = 'Stop'
$compiler = Join-Path $MsysBin 'g++.exe'
$lib = Join-Path $MsysBin '..\lib'
$raylib = Join-Path $lib 'libraylib.dll.a'
$runtime = @((Join-Path $MsysBin 'libraylib.dll'), (Join-Path $MsysBin 'glfw3.dll'),
             (Join-Path $MsysBin 'libwinpthread-1.dll'))
foreach ($file in (@($compiler, $raylib) + $runtime)) {
    if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
        throw "Komponen tidak ditemukan: $file. Lihat instalasi MSYS2 dan raylib di README.md."
    }
}

$out = Join-Path $PSScriptRoot 'out\windows'
New-Item -ItemType Directory -Force -Path $out | Out-Null
$exe = Join-Path $out 'mercusuar.exe'
$sources = @('main.cpp', 'assets.cpp', 'novel.cpp', 'ui.cpp') | ForEach-Object {
    Join-Path $PSScriptRoot "src\$_"
}
$flags = @('-std=c++17', '-O2', '-Wall', '-Wextra', '-Wpedantic', '-static-libgcc', '-static-libstdc++')
$oldPath = $env:Path
try {
    $env:Path = "$MsysBin;$oldPath"
    & $compiler @flags @sources $raylib -o $exe
    if ($LASTEXITCODE -ne 0) { throw "Build gagal (kode $LASTEXITCODE)." }
} finally {
    $env:Path = $oldPath
}
Copy-Item -LiteralPath $runtime -Destination $out -Force
"Selesai: $exe"
