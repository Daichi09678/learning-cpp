# Learning C++

Pilih contoh yang ingin kamu jalankan atau pelajari:

| Contoh | Mulai dari |
| --- | --- |
| **Mercusuar yang Menunggu** — visual novel C++ dan raylib dengan dua akhir | [Cara main dan build](visual-novel/README.md) · [Edit cerita](visual-novel/src/story.hpp) |
| **Tangkap Bintang** — game pixel Legacy BIOS yang dimainkan di QEMU | [Cara main dan build](bios-game/README.md) · [Kode game](bios-game/src/game.cpp) |
| **Halo Dunia** — latihan kompilasi C++ pertama | [Cara kompilasi dan penjelasan kode](hello-world/README.md) |

## Jalankan game

Buka folder `learning-c++` di VS Code, lalu pilih **Terminal > New Terminal** dan gunakan PowerShell. Jalankan salah satu perintah ini:

```powershell
# Mercusuar yang Menunggu
powershell -NoProfile -ExecutionPolicy Bypass -File .\visual-novel\run.ps1

# Tangkap Bintang
powershell -NoProfile -ExecutionPolicy Bypass -File .\bios-game\run.ps1
```

## Struktur proyek

```text
learning-c++/
├── visual-novel/
│   ├── assets/         # Gambar latar dan sprite karakter
│   ├── src/            # Kode, dialog, dan tampilan visual novel
│   └── out/windows/    # EXE dan DLL hasil build
├── bios-game/
│   ├── src/            # Game C++, Assembly, dan linker script
│   └── out/            # Image disket virtual hasil build
├── hello-world/
│   ├── src/            # Program C++ dasar
│   └── out/            # EXE hasil build
└── docs/               # Panduan belajar
```

## Panduan belajar

- [Membuat visual novel dengan C++](docs/PANDUAN-VISUAL-NOVEL.md): dialog, pilihan, aset, dan rancangan demo.
- [Memilih library, framework, atau engine game](docs/ALAT-PEMBUATAN-GAME.md): kapan memakai raylib, Ren'Py, Godot, atau alat lain.

## Jika `g++` atau `#include` bermasalah

- Jika terminal tidak mengenali `g++`, tambahkan `$env:Path = "C:\msys64\ucrt64\bin;$env:Path"` di sesi PowerShell yang sedang dipakai.
- Jika `#include` digarisbawahi di VS Code, buka **folder utama** `learning-c++` agar IntelliSense membaca [konfigurasi compiler](.vscode/c_cpp_properties.json). Bila perlu, tekan **Ctrl+Shift+P** dan pilih **Developer: Reload Window**.
