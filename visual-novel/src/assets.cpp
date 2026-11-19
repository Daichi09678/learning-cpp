#include "assets.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

namespace vn {
namespace {
namespace fs = std::filesystem;

Texture2D loadTexture(const fs::path& path) {
    const std::string filename = path.string();
    const Texture2D texture = LoadTexture(filename.c_str());
    if (texture.id) {
        SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    } else {
        std::cerr << "Gambar tidak bisa dibuka: " << filename << '\n';
    }
    return texture;
}

Font loadSystemFont(const char* filename) {
    const char* windowsFolder = std::getenv("WINDIR");
    if (!windowsFolder) {
        return GetFontDefault();
    }
    const std::string path = (fs::path(windowsFolder) / "Fonts" / filename).string();
    if (!FileExists(path.c_str())) {
        return GetFontDefault();
    }
    const Font font = LoadFontEx(path.c_str(), 72, nullptr, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    return font;
}
} // namespace

bool Assets::load(const std::filesystem::path& root) {
    const auto characters = root / "characters";
    coast = loadTexture(root / "backgrounds" / "image.png");
    sprites = {
        loadTexture(characters / "talking_mouth.png"),
        loadTexture(characters / "talking_mouth_blink.png"),
        loadTexture(characters / "talking_explain_mouth.png"),
        loadTexture(characters / "talking_explain_mouth_blink.png"),
    };
    if (coast.width < 32 || coast.height < 32 ||
        std::any_of(sprites.begin(), sprites.end(), [](Texture2D sprite) { return !sprite.id; })) {
        return false;
    }

    title = loadSystemFont("georgia.ttf");
    body = loadSystemFont("segoeui.ttf");
    label = loadSystemFont("seguisb.ttf");
    return true;
}

Assets::~Assets() {
    const auto defaultFontId = GetFontDefault().texture.id;
    for (const Font font : {title, body, label}) {
        if (font.texture.id && font.texture.id != defaultFontId) {
            UnloadFont(font);
        }
    }
    for (const auto sprite : sprites) {
        if (sprite.id) {
            UnloadTexture(sprite);
        }
    }
    if (coast.id) {
        UnloadTexture(coast);
    }
}

} // namespace vn
