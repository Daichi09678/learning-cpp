#pragma once

#include <raylib.h>

#include <array>
#include <cstddef>

namespace vn::layout {

inline constexpr int width = 1280;
inline constexpr int height = 720;
inline constexpr Rectangle canvas{0, 0, width, height};
inline constexpr Color background{13, 16, 34, 255};

inline constexpr Rectangle startButton{84, 440, 239, 62};
inline constexpr Rectangle menuButton{1100, 33, 132, 43};
inline constexpr Rectangle replayButton{84, 440, 249, 62};
inline constexpr Rectangle backButton{346, 440, 170, 62};
inline constexpr std::array<Rectangle, 2> optionBounds{{
    {94, 334, 478, 56},
    {94, 400, 478, 56},
}};

inline constexpr float dialogueSize = 29;
inline constexpr float dialogueWidth = 1060;

inline int optionAt(Vector2 point) {
    for (std::size_t i = 0; i < optionBounds.size(); ++i) {
        if (CheckCollisionPointRec(point, optionBounds[i])) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

} // namespace vn::layout
