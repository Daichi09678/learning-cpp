#pragma once

#include "story.hpp"

#include <raylib.h>

#include <string>

namespace vn {

enum class Screen { Title, Reading, Ending };

// Data yang diperlukan UI untuk menggambar satu frame.
struct NovelView {
    Screen screen;
    const story::Beat& beat;
    story::Coast previousCoast;
    float backgroundBlend;
    std::string dialogue;
    bool dialogueComplete;
    bool choiceBeat;
    int selectedOption;
};

class Novel {
public:
    explicit Novel(Font dialogueFont);

    void update(Vector2 mouse);
    NovelView view() const;

private:
    const story::Beat& beat() const;
    bool dialogueComplete() const;
    void start();
    void jump(story::Id destination);
    void updateChoice(Vector2 mouse, bool click, bool advance);

    Font dialogueFont_;
    Screen screen_ = Screen::Title;
    story::Id current_ = story::Id::Arrival;
    story::Coast previousCoast_ = story::Coast::Senja;
    std::string wrappedDialogue_;
    float revealed_ = 0;
    float sceneAge_ = 0;
    int selectedOption_ = 0;
};

} // namespace vn
