param([string]$MsysBin = 'C:\msys64\ucrt64\bin')

$ErrorActionPreference = 'Stop'
$clang = Join-Path $MsysBin 'clang++.exe'
$lld = Join-Path $MsysBin 'ld.lld.exe'
foreach ($tool in @($clang, $lld)) {
    if (-not (Test-Path -LiteralPath $tool -PathType Leaf)) {
        throw "Tool tidak ditemukan: $tool. Lihat petunjuk instalasi di README.md."
    }
}

function Invoke-BuildTool([string]$Tool, [string[]]$ToolArgs) {
    & $Tool @ToolArgs
    if ($LASTEXITCODE -ne 0) { throw "$Tool gagal (kode $LASTEXITCODE)." }
}

$out = Join-Path $PSScriptRoot 'out'
New-Item -ItemType Directory -Force -Path $out | Out-Null

Push-Location -LiteralPath $PSScriptRoot
try {
    $target = @('--target=i386-unknown-none-elf', '-march=i386', '-m16')
    Invoke-BuildTool $clang ($target + @('-c', 'src/boot.S', '-o', 'out/boot.o'))
    Invoke-BuildTool $clang ($target + @('-c', 'src/platform.S', '-o', 'out/platform.o'))
    Invoke-BuildTool $clang ($target + @('-std=c++17', '-Os', '-Wall', '-Wextra',
        '-ffreestanding', '-fno-builtin', '-fno-pic', '-fno-pie', '-fno-exceptions',
        '-fno-rtti', '-fno-stack-protector', '-fno-unwind-tables', '-fno-asynchronous-unwind-tables',
        '-c', 'src/game.cpp', '-o', 'out/game.o'))

    Invoke-BuildTool $lld @('-m', 'elf_i386', '--image-base=0', '--oformat=binary', '-Ttext=0x7c00',
        '-o', 'out/boot.bin', 'out/boot.o')
    Invoke-BuildTool $lld @('-m', 'elf_i386', '--image-base=0', '--oformat=binary', '-T', 'src/game.ld',
        '-o', 'out/game.bin', 'out/platform.o', 'out/game.o')
} finally {
    Pop-Location
}

$sectorSize = 512
$gameSectors = 16 # harus sama dengan boot.S dan batas alamat di game.ld
$boot = [IO.File]::ReadAllBytes((Join-Path $out 'boot.bin'))
$game = [IO.File]::ReadAllBytes((Join-Path $out 'game.bin'))
if ($boot.Length -ne $sectorSize -or $boot[510] -ne 0x55 -or $boot[511] -ne 0xaa) {
    throw 'Sektor boot harus 512 byte dengan tanda tangan 55 AA.'
}
if ($game.Length -eq 0 -or $game.Length -gt $sectorSize * $gameSectors) {
    throw 'Ukuran game di luar batas 16 sektor yang dibaca bootloader.'
}

$disk = [byte[]]::new(1440 * 1024)
[Array]::Copy($boot, 0, $disk, 0, $boot.Length)
[Array]::Copy($game, 0, $disk, $sectorSize, $game.Length)
$image = Join-Path $out 'tangkap-bintang.img'
[IO.File]::WriteAllBytes($image, $disk)
"Selesai: $image (kode game $($game.Length) byte)"
