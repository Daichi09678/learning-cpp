param([string]$Qemu = '')

$ErrorActionPreference = 'Stop'
if (-not $Qemu) {
    $command = Get-Command qemu-system-i386.exe -CommandType Application -ErrorAction SilentlyContinue
    if ($command) {
        $Qemu = $command.Source
    } else {
        $Qemu = Join-Path $env:ProgramFiles 'qemu\qemu-system-i386.exe'
    }
}
if (-not (Test-Path -LiteralPath $Qemu -PathType Leaf)) {
    throw "QEMU tidak ditemukan: $Qemu. Lihat petunjuk instalasi di README.md."
}

$image = Join-Path $PSScriptRoot 'out\tangkap-bintang.img'
if (-not (Test-Path -LiteralPath $image -PathType Leaf)) {
    throw 'Image game belum ada. Jalankan build.ps1 terlebih dahulu.'
}

& $Qemu -accel tcg -boot a -nic none -drive "if=floppy,format=raw,readonly=on,file=$($image.Replace(',', ',,'))"
if ($LASTEXITCODE -ne 0) { throw "QEMU gagal (kode $LASTEXITCODE)." }
