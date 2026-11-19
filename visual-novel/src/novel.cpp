#include "novel.hpp"

#include "layout.hpp"

#include <algorithm>
#include <sstream>

namespace vn {
namespace {
constexpr float charactersPerSecond = 48;
constexpr float backgroundFadeSeconds = 0.45f;
static_assert(story::choices.size() == layout::optionBounds.size());

std::string wrapDialogue(Font font, const char* dialogue) {
    std::istringstream words(dialogue);
    std::string word;
    std::string row;
    std::string result;
    while (words >> word) {
        const std::string candidate = row.empty() ? word : row + ' ' + word;
        const float width = MeasureTextEx(font, candidate.c_str(), layout::dialogueSize, 1).x;
        if (!row.empty() && width > layout::dialogueWidth) {
            result += row + '\n';
            row = word;
        } else {
            row = candidate;
        }
    }
    return result + row;
}
} // namespace

Novel::Novel(Font dialogueFont) : dialogueFont_(dialogueFont) {}

void Novel::update(Vector2 mouse) {
    const float dt = std::min(GetFrameTime(), 0.10f);
    const bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
    const bool advance = enter || IsKeyPressed(KEY_SPACE);

    if (screen_ == Screen::Title) {
        if (advance || (click && CheckCollisionPointRec(mouse, layout::startButton))) {
            start();
        }
        return;
    }

    sceneAge_ += dt;
    const auto menuButton = screen_ == Screen::Reading ? layout::menuButton : layout::backButton;
    if (IsKeyPressed(KEY_ESCAPE) || (click && CheckCollisionPointRec(mouse, menuButton))) {
        screen_ = Screen::Title;
        return;
    }
    if (screen_ == Screen::Ending) {
        if (advance || (click && CheckCollisionPointRec(mouse, layout::replayButton))) {
            start();
        }
        return;
    }

    const bool nextPage = advance || (click && CheckCollisionPointRec(mouse, layout::canvas));
    if (!dialogueComplete()) {
        const float length = static_cast<float>(wrappedDialogue_.size());
        revealed_ = nextPage ? length : std::min(revealed_ + dt * charactersPerSecond, length);
    } else if (current_ == story::Id::Decision) {
        updateChoice(mouse, click, advance);
    } else if (nextPage) {
        if (beat().ending) {
            screen_ = Screen::Ending;
        } else {
            jump(beat().next);
        }
    }
}

NovelView Novel::view() const {
    return {screen_, beat(), previousCoast_, std::min(sceneAge_ / backgroundFadeSeconds, 1.0f),
            wrappedDialogue_.substr(0, static_cast<std::size_t>(revealed_)), dialogueComplete(),
            current_ == story::Id::Decision, selectedOption_};
}

const story::Beat& Novel::beat() const { return story::get(current_); }

bool Novel::dialogueComplete() const {
    return revealed_ >= static_cast<float>(wrappedDialogue_.size());
}

void Novel::start() {
    screen_ = Screen::Reading;
    current_ = story::Id::Arrival;
    jump(story::Id::Arrival);
}

void Novel::jump(story::Id destination) {
    previousCoast_ = beat().coast;
    current_ = destination;
    sceneAge_ = 0;
    revealed_ = 0;
    selectedOption_ = 0;
    wrappedDialogue_ = wrapDialogue(dialogueFont_, beat().dialogue);
}

void Novel::updateChoice(Vector2 mouse, bool click, bool advance) {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        selectedOption_ = 1 - selectedOption_;
    }

    int option = click ? layout::optionAt(mouse) : -1;
    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
        option = 0;
    } else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
        option = 1;
    } else if (option < 0 && advance) {
        option = selectedOption_;
    }
    if (option >= 0) {
        jump(story::choices[option].target);
    }
}

} // namespace vn
