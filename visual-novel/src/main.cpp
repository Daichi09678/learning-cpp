#include "assets.hpp"
#include "layout.hpp"
#include "novel.hpp"
#include "ui.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>

namespace {
namespace layout = vn::layout;

int run() {
    vn::Assets art;
    // Aplikasi berada di visual-novel/out/windows; assets sejajar dengan out.
    const auto assets = std::filesystem::path(GetApplicationDirectory()) / ".." / ".." / "assets";
    if (!art.load(assets)) {
        return EXIT_FAILURE;
    }

    const RenderTexture2D canvas = LoadRenderTexture(layout::width, layout::height);
    if (!canvas.id) {
        return EXIT_FAILURE;
    }
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);
    vn::Novel novel(art.body);

    // Render texture berasal dari koordinat OpenGL; tinggi negatif membaliknya.
    constexpr Rectangle source{0, 0, layout::width, -layout::height};
    while (!WindowShouldClose()) {
        const float w = static_cast<float>(GetScreenWidth());
        const float h = static_cast<float>(GetScreenHeight());
        const float scale = std::min(w / layout::width, h / layout::height);
        if (scale <= 0) {
            BeginDrawing();
            EndDrawing();
            continue;
        }
        const Rectangle viewport{(w - layout::width * scale) / 2,
                                 (h - layout::height * scale) / 2,
                                 layout::width * scale, layout::height * scale};
        const Vector2 cursor = GetMousePosition();
        const Vector2 mouse{(cursor.x - viewport.x) / scale, (cursor.y - viewport.y) / scale};
        novel.update(mouse);

        BeginTextureMode(canvas);
        ClearBackground(layout::background);
        vn::drawNovel(art, novel.view(), mouse);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(layout::background);
        DrawTexturePro(canvas.texture, source, viewport, {0, 0}, 0, WHITE);
        EndDrawing();
    }
    UnloadRenderTexture(canvas);
    return EXIT_SUCCESS;
}
} // namespace

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1152, 648, "Mercusuar yang Menunggu");
    if (!IsWindowReady()) {
        return EXIT_FAILURE;
    }
    SetWindowMinSize(640, 360);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    const int status = run();
    CloseWindow();
    return status;
}
