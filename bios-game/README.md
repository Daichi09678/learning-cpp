# Tangkap Bintang

Game pixel C++ untuk Legacy BIOS. Kamu punya tiga nyawa dan menang setelah menangkap **10 bintang**. Gerakkan keranjang dengan **← / →** atau **A / D**. **Enter** memulai permainan dan **Esc** kembali ke menu.

## Jalankan di QEMU

Dari PowerShell di folder utama `learning-c++`:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\bios-game\run.ps1
```

Klik jendela QEMU, lalu tekan **Enter**. QEMU memulai komputer virtual dengan Legacy BIOS dan membaca `out/tangkap-bintang.img` sebagai disket virtual. Image ini dibuat untuk Legacy BIOS, bukan untuk boot UEFI langsung.

## Build ulang

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\bios-game\build.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File .\bios-game\run.ps1
```

Untuk pindah komputer Windows, pasang MSYS2 dan QEMU, lalu jalankan di terminal **MSYS2 UCRT64**:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-clang mingw-w64-ucrt-x86_64-lld
```

## Susunan kode

| File | Tugas |
| --- | --- |
| [`src/game.cpp`](src/game.cpp) | Aturan, gambar pixel, dan game loop. Mulai baca dari `pixel_main()`. |
| [`src/boot.S`](src/boot.S) | Kode pertama yang dijalankan BIOS. Ia memuat game dari disket virtual. |
| [`src/platform.S`](src/platform.S) | Penghubung C++ ke keyboard BIOS dan layar VGA 320 × 200. |
| [`src/game.ld`](src/game.ld) | Menentukan alamat memori tempat kode game dimuat. |

**Game loop** membaca tombol, memperbarui posisi bintang, mengecek tabrakan dengan keranjang atau tanah, lalu menggambar layar. Coba ubah `basketWidth` di `src/game.cpp` untuk melihat pengaruhnya pada tingkat kesulitan.
