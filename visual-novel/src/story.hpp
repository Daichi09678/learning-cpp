#pragma once

#include <array>
#include <cstddef>

namespace story {

enum class Coast { Senja, Malam, Pagi, Fajar };
enum class Pose { Hidden, Calm, Explain };
enum class Id {
    Arrival, Greeting, Promise, Signal, Decision,
    Walk, Lantern, Letter,
    Wait, Whisper, Sunrise,
    Count,
};

struct Beat {
    Coast coast;
    Pose pose;
    const char* speaker;
    const char* dialogue;
    Id next;
    const char* ending = nullptr;
    const char* endingTitle = nullptr;
};

inline constexpr std::array<Beat, static_cast<std::size_t>(Id::Count)> beats{{
    // Pembuka bersama, sampai pilihan di dermaga.
    {Coast::Senja, Pose::Hidden, "NARASI",
     "Angin asin menyelinap di sela papan dermaga. Di kejauhan, mercusuar yang sudah bertahun-tahun padam berkedip tiga kali.", Id::Greeting},
    {Coast::Senja, Pose::Calm, "NARA",
     "Kamu juga melihatnya? Tiga kedipan. Dulu ayahku selalu mengirim isyarat itu kalau kapalnya sudah dekat.", Id::Promise},
    {Coast::Senja, Pose::Explain, "NARA",
     "Sebelum berlayar terakhir kali, dia berjanji akan menyalakan lampu itu lagi. Aku datang ke sini setiap kali langit mulai gelap.", Id::Signal},
    {Coast::Malam, Pose::Calm, "NARASI",
     "Langit perlahan berubah ungu. Lampu itu berkedip lagi. Kali ini, Nara mengangkat lentera kecilnya untuk membalas.", Id::Decision},
    {Coast::Malam, Pose::Explain, "NARA",
     "Jalan ke mercusuar masih bisa dilewati sebelum pasang. Atau kita bisa tetap di sini sampai fajar. Kamu mau menemaniku ke mana?", Id::Count},
    // Cabang 1: memeriksa mercusuar.
    {Coast::Malam, Pose::Hidden, "NARASI",
     "Kalian berjalan menyusuri pantai. Pintu mercusuar terbuka, dan dari tangganya tercium bau kayu basah serta minyak lampu.", Id::Lantern},
    {Coast::Malam, Pose::Explain, "NARA",
     "Lentera tua ini masih hangat. Lihat, ada amplop di bawahnya. Tulisan di depannya... ini tulisan Ayah.", Id::Letter},
    {Coast::Malam, Pose::Calm, "NARASI",
     "Di dalam amplop tertulis: 'Kalau kamu membaca ini, berarti kamu sudah menemukan jalanmu sendiri.' Nara membaca kalimat itu sekali lagi, kali ini sambil tersenyum.", Id::Count,
     "AKHIR 01 / 02", "Lentera yang Hangat"},
    // Cabang 2: menunggu di dermaga.
    {Coast::Malam, Pose::Calm, "NARA",
     "Baiklah. Aku akan taruh lentera di ujung dermaga. Kalau ada yang sedang mencari jalan pulang, mereka bisa melihat kita dari laut.", Id::Whisper},
    {Coast::Fajar, Pose::Hidden, "NARASI",
     "Jauh setelah lampu mercusuar padam, ombak membawa sebuah botol kecil ke kaki dermaga. Di dalamnya ada secarik kertas yang masih kering.", Id::Sunrise},
    {Coast::Pagi, Pose::Calm, "NARA",
     "Ini bukan surat dari Ayah. Ini peta pulau yang pernah ia gambar untukku. Besok, aku mau naik perahu dan melihatnya sendiri.", Id::Count,
     "AKHIR 02 / 02", "Peta Saat Fajar"},
}};

struct Choice {
    const char* label;
    Id target;
};

inline constexpr std::array<Choice, 2> choices{{
    {"Periksa mercusuar", Id::Walk},
    {"Tunggu di dermaga", Id::Wait},
}};

constexpr const Beat& get(Id id) {
    return beats[static_cast<std::size_t>(id)];
}

} // namespace story
