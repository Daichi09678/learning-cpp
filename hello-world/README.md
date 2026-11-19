# Halo Dunia dengan C++

Contoh pertama untuk belajar menggunakan compiler GCC di Windows. Kode ada di [`src/main.cpp`](src/main.cpp).

## Kompilasi dan jalankan

Dari PowerShell di folder utama `learning-c++`:

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
g++ --version
New-Item -ItemType Directory -Force .\hello-world\out | Out-Null
g++ -std=c++17 -Wall -Wextra .\hello-world\src\main.cpp -o .\hello-world\out\hello-world.exe
.\hello-world\out\hello-world.exe
```

Hasilnya: `Halo, dunia C++!`

Perintah pertama menambahkan GCC ke PATH terminal ini. `-std=c++17` memilih standar C++17, `-Wall -Wextra` mengaktifkan peringatan, dan `-o` menentukan nama program. Setelah mengubah kode, kompilasi lagi sebelum menjalankan `.exe`.

## Cara kerja kode

- `#include <iostream>` menyediakan fasilitas input/output.
- `int main()` adalah titik awal program.
- `std::cout` menulis ke terminal, dan `\n` membuat baris baru.
- `return 0;` menandakan program selesai dengan sukses.
