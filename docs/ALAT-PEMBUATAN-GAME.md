# Memilih alat untuk membuat game

Framework tidak wajib. Compiler mengubah kode menjadi program. Library dan engine membantu dengan menyediakan fitur siap pakai, seperti gambar, input, suara, dan editor level.

| Tujuan | Alat yang cocok | Yang dibantu |
| --- | --- | --- |
| Belajar C++ dengan game 2D desktop | **raylib** (library) | Jendela, gambar, input, dan suara. Kamu menulis aturan dan game loop. |
| Visual novel tradisional | **Ren'Py** (engine) | Dialog, pilihan, save/load, dan kontrol membaca. |
| Level, animasi, atau editor visual | **Godot**, **Unity**, atau **Unreal Engine** (engine) | Editor, aset, animasi, fisika, dan ekspor. Godot biasa memakai GDScript/C#, Unity memakai C#, dan Unreal mendukung C++ serta Blueprint. |
| Game 2D di browser | **Phaser** (framework JavaScript/TypeScript) | Gambar, input, dan pengelolaan scene. Next.js dapat membantu membuat situs pendamping, seperti halaman login atau papan skor. |
| Eksperimen game saat boot | **C++ + Assembly + emulator** | Bootstrap dan akses langsung ke VGA serta keyboard BIOS, seperti di [Tangkap Bintang](../bios-game/README.md). |

Untuk belajar cara membuat visual novel, baca [Panduan Visual Novel dengan C++](PANDUAN-VISUAL-NOVEL.md), lalu coba demo [Mercusuar yang Menunggu](../visual-novel/README.md).
