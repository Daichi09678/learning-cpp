#pragma once

#include <raylib.h>

namespace vn {

struct Assets;
struct NovelView;

void drawNovel(const Assets& art, const NovelView& view, Vector2 mouse);

} // namespace vn
