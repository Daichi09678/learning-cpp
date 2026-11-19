#pragma once

#include <raylib.h>

#include <array>
#include <filesystem>

namespace vn {

struct Assets {
    Texture2D coast{};
    std::array<Texture2D, 4> sprites{};
    Font title{};
    Font body{};
    Font label{};

    Assets() = default;
    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;
    ~Assets();

    bool load(const std::filesystem::path& root);
};

} // namespace vn
