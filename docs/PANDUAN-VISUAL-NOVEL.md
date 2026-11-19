# Panduan membuat visual novel dengan C++

Demo yang sudah bisa dimainkan: [Mercusuar yang Menunggu](../visual-novel/README.md).

**C++ bisa dipakai untuk membuat visual novel seperti *Umineko* atau *ATRI*.** Kalau tujuanmu belajar C++, pilihan yang praktis adalah **C++ + raylib**. Kalau tujuanmu menyelesaikan dan merilis cerita secepat mungkin, **Ren'Py** biasanya lebih praktis karena sudah punya banyak fitur visual novel.

## 1. Apa yang membuat sebuah game menjadi visual novel?

Visual novel menyampaikan cerita lewat dialog dan narasi di layar, biasanya ditemani gambar latar, karakter, musik, dan efek suara. Kamu bisa mengambil inspirasi dari cara *Umineko* membangun misteri lewat narasi dan musik, atau dari penyajian adegan emosional di *ATRI* lewat ekspresi karakter, ilustrasi, dan audio.

Visual novel bisa **linear**, yaitu pemain membaca cerita dengan urutan yang sudah ditentukan. Ia juga bisa **bercabang**, yaitu pilihan pemain mengubah dialog, adegan, rute, atau akhir cerita. Bahkan di cerita bercabang, pilihan kecil boleh bergabung lagi ke adegan yang sama. Untuk prototipe pertama, satu cerita pendek dengan satu keputusan sudah cukup untuk belajar keduanya.

Berbeda dengan game BIOS di proyek ini, visual novel desktop biasanya dibuka seperti aplikasi Windows biasa dari file `.exe`. Ide **game loop**-nya tetap sama: baca input, perbarui keadaan, dan gambar layar. Keadaan yang diperbarui sekarang adalah posisi pembaca dalam cerita, bukan posisi bintang dan keranjang.

## 2. Pilih alat sesuai tujuanmu

| Alat | Yang sudah disediakan | Kapan sebaiknya dipilih |
| --- | --- | --- |
| **C++ + raylib** | Jendela, input, gambar, teks, dan audio. Kamu membuat sistem dialog, pilihan, menu, dan penyimpanan. | Ingin belajar C++ sambil memahami cara visual novel bekerja dari dalam. |
| **Ren'Py** | Sistem dialog, pilihan, menu, save/load, dan fitur membaca seperti auto dan skip. Cerita biasanya ditulis dalam skrip `.rpy`, dengan Python bila diperlukan. | Ingin fokus menulis cerita, membuat ilustrasi, dan menyelesaikan visual novel tradisional lebih cepat. |
| **Godot** | Editor visual, UI, animasi, dan audio. Kamu bisa membuat sistem dialog sendiri atau memakai plugin. GDScript adalah salah satu cara termudah untuk mulai. | Ingin membuat visual novel yang dicampur dengan eksplorasi, teka-teki, atau minigame. |

C++ adalah **bahasa pemrograman**, GCC adalah **compiler**, dan raylib adalah **library** yang dipanggil dari kode C++. Ren'Py dan Godot adalah **engine** yang menyediakan lebih banyak sistem siap pakai. Untuk jalur C++ di Windows, GCC dari MSYS2 UCRT64 yang sudah terpasang bisa dipakai; raylib adalah dependensi tambahan yang akan dipasang ketika mulai membuat prototipe.

## 3. Pisahkan aturan permainan dari isi cerita

Sebaiknya kode C++ tahu **cara** menampilkan dialog, sedangkan data cerita menentukan **dialog apa** yang ditampilkan. Kalau nanti ingin mengganti nama tokoh atau menulis bab baru, kamu cukup mengubah data cerita tanpa mengutak-atik kode untuk tombol dan gambar.

Satu **node** adalah satu titik dalam cerita. Contoh di bawah ini menunjukkan pembuka, pilihan, dan dua akhir. Format ini adalah contoh data JSON; aplikasi C++ tetap perlu membaca data tersebut dan menentukan perilaku setiap field.

```json
{
  "start": "dermaga",
  "nodes": {
    "dermaga": {
      "background": "dermaga_senja.png",
      "speaker": "Nara",
      "text": "Lampu mercusuar menyala. Padahal sudah lama ditinggalkan.",
      "next": "keputusan"
    },
    "keputusan": {
      "text": "Apa yang kamu lakukan?",
      "choices": [
        { "label": "Periksa mercusuar", "target": "akhir_mercusuar" },
        { "label": "Tunggu di dermaga", "target": "akhir_dermaga" }
      ]
    },
    "akhir_mercusuar": {
      "text": "Di balik pintu, kamu mendengar seseorang menyebut namamu.",
      "ending": true
    },
    "akhir_dermaga": {
      "text": "Saat fajar tiba, lampu itu padam dengan sendirinya.",
      "ending": true
    }
  }
}
```

Saat pemain mulai, game membaca node `dermaga`. Ketika pemain melanjutkan dialog, game mengikuti `next` menuju `keputusan`. Di situ, tombol biasa diganti dengan daftar pilihan. Memilih opsi pertama mengubah node aktif menjadi `akhir_mercusuar`. Setelah akhir cerita selesai dibaca, game bisa menampilkan tombol kembali ke menu.

Untuk percobaan pertama, dialog bisa berupa daftar sederhana di C++. Pindah ke JSON atau format skrip lain saat ceritanya mulai panjang. Pustaka standar C++ belum menyediakan *parser* JSON, jadi jika memilih JSON kamu bisa pakai library seperti **nlohmann/json**. Saat memuat cerita, periksa apakah semua `next` dan `target` merujuk ke ID yang ada.

## 4. Apa yang perlu diprogram?

| Sistem | Cara kerjanya |
| --- | --- |
| **Tampilan adegan** | Gambar berlapis: latar → sprite karakter → efek → kotak dialog dan tombol. Video dan animasi rumit bisa menyusul. |
| **Dialog** | Tampilkan nama, teks, dan tombol lanjut. Jika teks muncul huruf demi huruf, klik pertama bisa menampilkan seluruh kalimat dan klik berikutnya baru lanjut. Atur lebar kotak agar teks panjang turun ke baris berikutnya. |
| **Pilihan dan keadaan cerita** | Simpan ID node aktif. Jika sebuah keputusan perlu diingat, simpan juga variabel seperti `menemukanSurat = true`; adegan berikutnya bisa mengecek variabel itu. |
| **Audio** | Musik latar bisa berulang, suara pintu diputar sekali, dan sulih suara dikaitkan dengan baris dialog. Sediakan volume terpisah jika proyek sudah berkembang. |
| **Save/load** | Simpan ID node, variabel cerita, dan informasi yang diperlukan untuk memulihkan adegan; misalnya latar, sprite, dan musik jika diwarisi dari node sebelumnya. Gunakan ID yang stabil agar penyisipan dialog tidak langsung membuat save lama salah posisi. |
| **UI membaca** | Mulai dari lanjut dialog, pilihan, dan kembali ke menu. Tambahkan *backlog* (riwayat teks), auto, dan skip dialog yang sudah dibaca setelah alur inti stabil. |

Untuk teks yang mudah dibaca, gunakan font yang mencakup karakter bahasa target, tentukan ukuran teks yang nyaman, dan cek keterbacaan kotak dialog di atas latar terang maupun gelap. Uji juga dengan keyboard, terutama saat memilih opsi. Untuk audio, satu musik dan satu efek suara yang ditempatkan pada momen yang tepat sering lebih efektif daripada banyak suara dalam setiap adegan.

## 5. Cara mengerjakannya dari awal

1. **Tulis cerita 2–5 menit.** Pilih satu lokasi, satu atau dua tokoh, dan satu keputusan. Gambar peta alurnya sebelum menulis banyak dialog.
2. **Buat layar dialog.** Buka jendela raylib, gambar satu latar dan kotak dialog, lalu buat **Enter** atau klik untuk maju.
3. **Buat satu pilihan bercabang.** Pastikan kedua pilihan berakhir di tempat yang benar dan pemain bisa kembali ke menu.
4. **Tambahkan tampilan dan suara.** Gunakan gambar sementara dulu. Begitu alurnya enak dibaca, ganti dengan latar, ekspresi karakter, musik, dan efek yang sesuai.
5. **Tambahkan satu slot save/load.** Coba simpan, tutup aplikasi, buka lagi, dan pastikan cerita kembali ke baris dan cabang yang sama.
6. **Uji dan kemas demo.** Mainkan semua cabang, cek teks panjang dan file aset yang hilang, lalu coba paket di komputer lain. Distribusikan aset dan dependensi yang benar-benar diperlukan bersama program.

Untuk aset, kamu bisa membagi folder menjadi `assets/backgrounds`, `assets/characters`, `assets/audio`, dan `assets/fonts`. Pakai karya sendiri, aset berlisensi yang sesuai, atau aset yang kamu punya izin untuk gunakan. Ambil inspirasi dari cara *Umineko* dan *ATRI* membangun suasana; buat tokoh, tulisan, gambar, dan suara proyekmu sendiri atau gunakan yang berizin.

## Target pertama yang masuk akal

**Satu layar dengan latar dermaga, satu sprite, kotak dialog, dan dua tombol pilihan yang menuju dua akhir.** Ini sudah menjadi visual novel mini yang utuh. Setelah itu, barulah pilih fitur berikutnya berdasarkan cerita: ilustrasi khusus (CG), galeri, bab yang terkunci, atau rute yang terbuka setelah akhir tertentu.
