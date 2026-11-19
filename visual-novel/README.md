# Mercusuar yang Menunggu

Visual novel C++ dan raylib tentang Nara dan sebuah lampu di seberang dermaga. Gambar pantai dipakai sebagai empat latar. Empat sprite Nara menampilkan dua pose, masing-masing dengan animasi kedip. Pilihan di dermaga membawa cerita ke salah satu dari dua akhir.

## Jalankan demo

Dari PowerShell di folder utama `learning-c++`:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\visual-novel\run.ps1
```

- **Enter**, **Spasi**, atau **klik** untuk membaca. Jika teks masih muncul, tekan sekali untuk menampilkan semuanya, lalu tekan lagi untuk lanjut.
- Saat pilihan terlihat, klik salah satunya, tekan **1 / 2**, atau gunakan **↑ / ↓** lalu **Enter**.
- Tekan **Esc** untuk kembali ke menu. Pilih **Main lagi** setelah mencapai akhir untuk mencoba cabang lain.

## Di mana mengeditnya?

| Lokasi | Isi |
| --- | --- |
| [`src/story.hpp`](src/story.hpp) | Dialog, latar dan pose per adegan, pilihan, serta kedua akhir. Mulai dari sini kalau ingin mengubah cerita. |
| [`src/novel.cpp`](src/novel.cpp) | Input, teks yang muncul bertahap, dan perpindahan antaradegan. |
| [`src/ui.cpp`](src/ui.cpp) dan [`src/layout.hpp`](src/layout.hpp) | Tampilan menu dan dialog, posisi tombol, serta cara menggambar latar dan sprite. |
| [`src/assets.cpp`](src/assets.cpp) | Memuat gambar dan font. |
| [`src/main.cpp`](src/main.cpp) | Jendela dan game loop. |
| [`assets/backgrounds/`](assets/backgrounds/) dan [`assets/characters/`](assets/characters/) | Gambar pantai dan sprite aslinya. |

## Build ulang

Setelah mengedit kode, jalankan dari folder utama `learning-c++`:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\visual-novel\run.ps1 -Rebuild
```

Untuk menyiapkan compiler dan raylib di komputer lain, pasang MSYS2 lalu jalankan di terminal **MSYS2 UCRT64**:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-raylib
```

File `out/windows/mercusuar.exe` bisa dibuka langsung. Simpan DLL di samping `.exe` dan biarkan `assets/` sejajar dengan `out/`. Tutup jendela versi ini sebelum menjalankan build ulang agar Windows bisa mengganti `.exe` dan DLL.
