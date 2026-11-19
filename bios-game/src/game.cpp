extern "C" int bios_key();
extern "C" void canvas_span(int offset, int count, int color);
extern "C" void vga_present();

namespace {
constexpr int width = 320;
constexpr int height = 200;
constexpr int ground = 190;
constexpr int basketY = 174;
constexpr int basketWidth = 30;
constexpr int basketHeight = 9;
constexpr int starSize = 9;
constexpr int goal = 10;

enum Color {
    sky = 1,
    dimBlue = 3,
    grass = 10,
    cyan = 11,
    red = 12,
    yellow = 14,
    white = 15,
};

struct Game {
    int basketX;
    int starX;
    int starY;
    int score;
    int lives;
    unsigned int random;
};

enum class Screen { title, playing, won, lost };

// Font 5x7: setiap byte berisi satu kolom, dan bit 0 adalah pixel paling atas.
constexpr unsigned char letters[][5] = {
    {0x7e,0x11,0x11,0x11,0x7e}, {0x7f,0x49,0x49,0x49,0x36}, // A B
    {0x3e,0x41,0x41,0x41,0x22}, {0x7f,0x41,0x41,0x22,0x1c}, // C D
    {0x7f,0x49,0x49,0x49,0x41}, {0x7f,0x09,0x09,0x09,0x01}, // E F
    {0x3e,0x41,0x49,0x49,0x7a}, {0x7f,0x08,0x08,0x08,0x7f}, // G H
    {0x00,0x41,0x7f,0x41,0x00}, {0x20,0x40,0x41,0x3f,0x01}, // I J
    {0x7f,0x08,0x14,0x22,0x41}, {0x7f,0x40,0x40,0x40,0x40}, // K L
    {0x7f,0x02,0x0c,0x02,0x7f}, {0x7f,0x04,0x08,0x10,0x7f}, // M N
    {0x3e,0x41,0x41,0x41,0x3e}, {0x7f,0x09,0x09,0x09,0x06}, // O P
    {0x3e,0x41,0x51,0x21,0x5e}, {0x7f,0x09,0x19,0x29,0x46}, // Q R
    {0x46,0x49,0x49,0x49,0x31}, {0x01,0x01,0x7f,0x01,0x01}, // S T
    {0x3f,0x40,0x40,0x40,0x3f}, {0x1f,0x20,0x40,0x20,0x1f}, // U V
    {0x3f,0x40,0x38,0x40,0x3f}, {0x63,0x14,0x08,0x14,0x63}, // W X
    {0x07,0x08,0x70,0x08,0x07}, {0x61,0x51,0x49,0x45,0x43}, // Y Z
};
constexpr unsigned char digits[][5] = {
    {0x3e,0x51,0x49,0x45,0x3e}, {0x00,0x42,0x7f,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46}, {0x21,0x41,0x45,0x4b,0x31},
    {0x18,0x14,0x12,0x7f,0x10}, {0x27,0x45,0x45,0x45,0x39},
    {0x3c,0x4a,0x49,0x49,0x30}, {0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36}, {0x06,0x49,0x49,0x29,0x1e},
};
constexpr unsigned char slash[] = {0x60,0x10,0x08,0x04,0x03};

void rect(int x, int y, int w, int h, int color) {
    for (int row = 0; row < h; ++row) {
        canvas_span((y + row) * width + x, w, color);
    }
}

void text(int x, int y, const char* label, int color, int scale = 1) {
    for (; *label; ++label, x += 6 * scale) {
        const char ch = *label;
        const unsigned char* columns = nullptr;
        if (ch >= 'A' && ch <= 'Z') columns = letters[ch - 'A'];
        if (ch >= '0' && ch <= '9') columns = digits[ch - '0'];
        if (ch == '/') columns = slash;
        if (!columns) continue;

        for (int col = 0; col < 5; ++col) {
            for (int row = 0; row < 7; ++row) {
                if (columns[col] & (1 << row)) {
                    rect(x + col * scale, y + row * scale, scale, scale, color);
                }
            }
        }
    }
}

void centered(int y, const char* label, int color, int scale = 1) {
    int length = 0;
    while (label[length]) ++length;
    text((width - (6 * length - 1) * scale) / 2, y, label, color, scale);
}

void drawStar(int x, int y, int scale = 1) {
    rect(x + 4 * scale, y, scale, 9 * scale, yellow);
    rect(x + 2 * scale, y + 2 * scale, 5 * scale, 5 * scale, yellow);
    rect(x, y + 4 * scale, 9 * scale, scale, yellow);
    rect(x + 4 * scale, y + 4 * scale, scale, scale, white);
}

void drawBackground() {
    canvas_span(0, width * height, sky);
    for (int i = 0; i < 32; ++i) {
        rect(8 + (i * 73) % 304, 22 + (i * 29) % 162, 1, 1, dimBlue);
    }
    rect(0, ground, width, 2, grass);
    rect(0, ground + 2, width, height - ground - 2, dimBlue);
}

unsigned int ticks() {
    // BIOS memperbarui nilai ini sekitar 18 kali per detik.
    return *reinterpret_cast<volatile const unsigned int*>(0x046c);
}

void nextStar(Game& game) {
    game.random = game.random * 1664525u + 1013904223u;
    game.starX = 8 + static_cast<int>((game.random >> 16) % (width - starSize - 16));
    game.starY = 21;
}

void start(Game& game) {
    game.basketX = (width - basketWidth) / 2;
    game.score = 0;
    game.lives = 3;
    game.random = ticks();
    nextStar(game);
}

void move(Game& game, int key) {
    const int ascii = key & 0xff;
    const int scan = key >> 8;
    if (scan == 0x4b || ascii == 'a' || ascii == 'A') game.basketX -= 15;
    if (scan == 0x4d || ascii == 'd' || ascii == 'D') game.basketX += 15;
    if (game.basketX < 0) game.basketX = 0;
    if (game.basketX > width - basketWidth) game.basketX = width - basketWidth;
}

void update(Game& game) {
    game.starY += 3 + game.score / 4;

    const bool overlapX = game.starX < game.basketX + basketWidth &&
                          game.starX + starSize > game.basketX;
    const bool overlapY = game.starY + starSize > basketY &&
                          game.starY < basketY + basketHeight;
    if (overlapX && overlapY) {
        ++game.score;
        nextStar(game);
    } else if (game.starY + starSize >= ground) {
        --game.lives;
        nextStar(game);
    }
}

void drawGame(const Game& game) {
    drawBackground();
    char score[] = "SKOR 00/10";
    score[5] = static_cast<char>('0' + game.score / 10);
    score[6] = static_cast<char>('0' + game.score % 10);
    text(8, 6, score, white);
    text(233, 6, "NYAWA", white);
    for (int i = 0; i < 3; ++i) {
        rect(268 + i * 14, 6, 10, 7, i < game.lives ? red : dimBlue);
    }
    rect(0, 18, width, 1, dimBlue);
    drawStar(game.starX, game.starY);
    rect(game.basketX, basketY, basketWidth, 2, cyan);
    rect(game.basketX + 2, basketY + 2, basketWidth - 4, basketHeight - 2, dimBlue);
    rect(game.basketX + 2, basketY + 6, basketWidth - 4, 3, cyan);
    vga_present();
}

void drawMenu(Screen screen) {
    drawBackground();
    drawStar(146, 24, 3);
    if (screen == Screen::title) {
        centered(68, "TANGKAP BINTANG", yellow, 3);
        centered(108, "PANAH KIRI KANAN ATAU A D", white);
        centered(126, "TANGKAP 10 BINTANG", white);
        centered(153, "ENTER MULAI", cyan, 2);
    } else {
        const bool won = screen == Screen::won;
        centered(68, won ? "KAMU MENANG" : "NYAWA HABIS", won ? yellow : red, 3);
        centered(120, "ENTER MAIN LAGI", cyan, 2);
        centered(154, "ESC MENU", white);
    }
    vga_present();
}
} // namespace

extern "C" [[noreturn]] void pixel_main() {
    Game game;
    Screen screen = Screen::title;
    drawMenu(screen);

    for (;;) {
        const unsigned int lastTick = ticks();
        while (const int key = bios_key()) {
            const int ascii = key & 0xff;
            if (ascii == 27 && screen != Screen::title) {
                screen = Screen::title;
                drawMenu(screen);
            } else if (ascii == 13 && screen != Screen::playing) {
                start(game);
                screen = Screen::playing;
            } else if (screen == Screen::playing) {
                move(game, key);
            }
        }

        if (screen == Screen::playing) {
            update(game);
            if (game.score >= goal || game.lives == 0) {
                screen = game.score >= goal ? Screen::won : Screen::lost;
                drawMenu(screen);
            } else {
                drawGame(game);
            }
        }

        while (ticks() == lastTick) __asm__ volatile ("hlt");
    }
}
