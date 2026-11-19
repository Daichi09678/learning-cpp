#include "ui.hpp"

#include "assets.hpp"
#include "layout.hpp"
#include "novel.hpp"

#include <algorithm>
#include <cmath>

namespace vn {
namespace {
using story::Coast;
using story::Pose;

constexpr Color gold{249, 203, 148, 255};
constexpr Color ivory{255, 245, 230, 255};
constexpr Color muted{220, 218, 229, 255};
constexpr Color ink{22, 24, 41, 255};

void print(Font font, const char* text, float x, float y, float size, Color color,
           float spacing = 1) {
    DrawTextEx(font, text, {x, y}, size, spacing, color);
}

void drawCoast(const Assets& art, Coast view, Color tint = WHITE) {
    // Setiap latar memakai satu kuadran; lewati garis putih di tengah gambar.
    const float midX = art.coast.width / 2.0f;
    const float midY = art.coast.height / 2.0f;
    const auto index = static_cast<int>(view);
    const Rectangle tile{index % 2 ? midX + 4 : 0, index / 2 ? midY + 4 : 0,
                         midX - 4, midY - 4};
    const float scale = std::max(layout::width / tile.width, layout::height / tile.height);
    const float cropW = layout::width / scale;
    const float cropH = layout::height / scale;
    const Rectangle source{tile.x + (tile.width - cropW) / 2, tile.y + (tile.height - cropH) / 2,
                           cropW, cropH};
    DrawTexturePro(art.coast, source, layout::canvas, {0, 0}, 0, tint);
}

void drawNara(const Assets& art, Pose pose, Rectangle place) {
    if (pose == Pose::Hidden) {
        return;
    }
    const bool blink = std::fmod(GetTime() + 0.4, 3.7) < 0.15;
    const auto frame = art.sprites[(pose == Pose::Explain ? 2 : 0) + (blink ? 1 : 0)];
    const Rectangle source{0, 0, static_cast<float>(frame.width), static_cast<float>(frame.height)};
    DrawTexturePro(frame, source, place, {0, 0}, 0, WHITE);
}

void button(const Assets& art, Rectangle bounds, const char* text, const char* hint,
            Vector2 mouse, bool primary = false) {
    const bool hover = CheckCollisionPointRec(mouse, bounds);
    const bool filled = primary || hover;
    const Color fill = filled ? (hover ? ivory : gold) : Fade(layout::background, 0.86f);
    DrawRectangleRounded(bounds, 0.18f, 8, fill);
    DrawRectangleRoundedLinesEx(bounds, 0.18f, 8, 1, Fade(gold, filled ? 0.85f : 0.50f));
    print(art.label, text, bounds.x + 16, bounds.y + (bounds.height - 22) / 2, 22,
          filled ? ink : ivory);
    const float hintWidth = MeasureTextEx(art.label, hint, 15, 1).x;
    print(art.label, hint, bounds.x + bounds.width - hintWidth - 16,
          bounds.y + (bounds.height - 15) / 2, 15, filled ? ink : muted);
}

void edgeShade() {
    DrawRectangleGradientV(0, 0, layout::width, 125, Fade(layout::background, 0.76f), BLANK);
    DrawRectangleGradientV(0, 360, layout::width, 360, BLANK, Fade(layout::background, 0.98f));
}

void leftShade() {
    DrawRectangleGradientH(0, 0, layout::width, layout::height,
                           Fade(layout::background, 0.98f), Fade(layout::background, 0.06f));
    DrawRectangleGradientV(0, 555, layout::width, 165, BLANK, Fade(layout::background, 0.88f));
}

void eyebrow(const Assets& art, const char* text, float x, float y) {
    DrawRectangle(static_cast<int>(x), static_cast<int>(y + 9), 28, 1, gold);
    print(art.label, text, x + 41, y, 16, gold, 2);
}

void drawTitle(const Assets& art, Vector2 mouse) {
    drawCoast(art, Coast::Malam);
    drawNara(art, Pose::Calm, {693, 136, 604, 604});
    leftShade();
    eyebrow(art, "VISUAL NOVEL   /   DEMO", 84, 106);
    print(art.title, "Mercusuar", 78, 160, 80, ivory);
    print(art.title, "yang Menunggu", 78, 247, 80, ivory);
    print(art.body, "Satu lampu menyala. Seseorang masih menunggu.", 84, 363, 27, muted);
    button(art, layout::startButton, "Mulai cerita", "ENTER", mouse, true);
    print(art.label, "PROLOG 01   /   DERMAGA", 84, 663, 16, gold, 2);
}

void drawEnding(const Assets& art, const story::Beat& beat, Vector2 mouse) {
    leftShade();
    eyebrow(art, beat.ending, 84, 158);
    print(art.title, beat.endingTitle, 78, 224, 67, ivory);
    print(art.body, "Setiap pilihan menyimpan cerita yang berbeda.", 84, 340, 26, muted);
    button(art, layout::replayButton, "Main lagi", "ENTER", mouse, true);
    button(art, layout::backButton, "Menu", "ESC", mouse);
}

void drawChoices(const Assets& art, int selected, Vector2 mouse) {
    eyebrow(art, "PILIH JALAN CERITAMU", 94, 301);
    const int hovered = layout::optionAt(mouse);
    const auto active = static_cast<std::size_t>(hovered >= 0 ? hovered : selected);
    for (std::size_t i = 0; i < story::choices.size(); ++i) {
        button(art, layout::optionBounds[i], story::choices[i].label,
               i == 0 ? "1" : "2", mouse, i == active);
    }
}

const char* dialogueHint(const NovelView& view) {
    if (!view.dialogueComplete) {
        return "KLIK / ENTER UNTUK BACA LANGSUNG";
    }
    return view.choiceBeat ? "PILIH DENGAN KLIK / 1 / 2" : "KLIK / ENTER UNTUK LANJUT";
}

void drawDialogue(const Assets& art, const NovelView& view) {
    const Rectangle box{63, 495, 1154, 182};
    DrawRectangleRounded(box, 0.11f, 10, Fade(layout::background, 0.93f));
    DrawRectangleRoundedLinesEx(box, 0.11f, 10, 1, Fade(gold, 0.35f));
    const Rectangle name{91, 479, 119, 33};
    DrawRectangleRounded(name, 0.25f, 8, gold);
    const float nameWidth = MeasureTextEx(art.label, view.beat.speaker, 18, 2).x;
    print(art.label, view.beat.speaker, name.x + (name.width - nameWidth) / 2, 486, 18, ink, 2);

    print(art.body, view.dialogue.c_str(), 91, 526, layout::dialogueSize, ivory);
    DrawLine(92, 629, 1187, 629, Fade(gold, 0.26f));
    print(art.label, "MERCUSUAR YANG MENUNGGU", 92, 644, 14, muted, 2);
    const char* hint = dialogueHint(view);
    const float right = MeasureTextEx(art.label, hint, 15, 1).x;
    print(art.label, hint, 1187 - right, 644, 15, gold);
}
} // namespace

void drawNovel(const Assets& art, const NovelView& view, Vector2 mouse) {
    if (view.screen == Screen::Title) {
        drawTitle(art, mouse);
        return;
    }

    if (view.previousCoast != view.beat.coast && view.backgroundBlend < 1) {
        drawCoast(art, view.previousCoast);
        drawCoast(art, view.beat.coast, Fade(WHITE, view.backgroundBlend));
    } else {
        drawCoast(art, view.beat.coast);
    }
    drawNara(art, view.beat.pose, {651, 117, 622, 622});

    if (view.screen == Screen::Ending) {
        drawEnding(art, view.beat, mouse);
        return;
    }

    edgeShade();
    eyebrow(art, "01   /   DERMAGA", 49, 46);
    button(art, layout::menuButton, "Menu", "ESC", mouse);
    if (view.choiceBeat && view.dialogueComplete) {
        drawChoices(art, view.selectedOption, mouse);
    }
    drawDialogue(art, view);
}

} // namespace vn
