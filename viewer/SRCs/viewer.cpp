#include "viewer.hpp"
#include <iostream>
#include <random>

namespace emulator {

viewer::viewer(int width, int height)
    : width(width), height(height),
      hConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE)),
      charBuffer(width * height, ' '),
      attrBuffer(width * height, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
{
    initializeConsole();
}

viewer::~viewer() {
    setCursorVisible(true);
}

void viewer::initializeConsole() {
    setConsoleBufferSize(width, height);
    setConsoleWindowSize(width, height);
    setCursorVisible(false);
    clearBuffer();
    clearScreen();
}

WORD viewer::toConsoleAttribute(Color color) {
    switch (color) {
        case Color::BLACK: return 0;
        case Color::BLUE: return FOREGROUND_BLUE;
        case Color::GREEN: return FOREGROUND_GREEN;
        case Color::CYAN: return FOREGROUND_BLUE | FOREGROUND_GREEN;
        case Color::RED: return FOREGROUND_RED;
        case Color::MAGENTA: return FOREGROUND_RED | FOREGROUND_BLUE;
        case Color::YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
        case Color::WHITE: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case Color::BRIGHT_GREEN: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::BRIGHT_CYAN: return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::BRIGHT_RED: return FOREGROUND_RED | FOREGROUND_INTENSITY;
        case Color::BRIGHT_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case Color::DEFAULT:
        default:
            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

void viewer::setCursorVisible(bool visible) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = visible ? TRUE : FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

void viewer::clearScreen() {
    clearConsole();
}

void viewer::clearBuffer() {
    std::fill(charBuffer.begin(), charBuffer.end(), ' ');
    std::fill(attrBuffer.begin(), attrBuffer.end(), toConsoleAttribute(Color::DEFAULT));
}

void viewer::drawChar(int col, int row, char ch, Color color) {
    if (col < 0 || col >= width || row < 0 || row >= height) return;
    int idx = row * width + col;
    charBuffer[idx] = ch;
    attrBuffer[idx] = toConsoleAttribute(color);
}

void viewer::drawString(int col, int row, const std::string& str, Color color) {
    if (row < 0 || row >= height) return;
    for (size_t i = 0; i < str.length(); ++i) {
        int c = col + static_cast<int>(i);
        if (c >= 0 && c < width) {
            int idx = row * width + c;
            charBuffer[idx] = str[i];
            attrBuffer[idx] = toConsoleAttribute(color);
        }
    }
}

void viewer::drawBox(int startCol, int startRow, int boxWidth, int boxHeight, char border, Color color) {
    for (int r = startRow; r < startRow + boxHeight; ++r) {
        for (int c = startCol; c < startCol + boxWidth; ++c) {
            if (r == startRow || r == startRow + boxHeight - 1 ||
                c == startCol || c == startCol + boxWidth - 1) {
                drawChar(c, r, border, color);
            }
        }
    }
}

void viewer::fillRect(int startCol, int startRow, int rectWidth, int rectHeight, char fill, Color color) {
    for (int r = startRow; r < startRow + rectHeight; ++r) {
        for (int c = startCol; c < startCol + rectWidth; ++c) {
            drawChar(c, r, fill, color);
        }
    }
}

void viewer::present() {
    COORD bufferCoord = {0, 0};
    DWORD written = 0;

    WriteConsoleOutputCharacterA(
        hConsoleOutput,
        charBuffer.data(),
        static_cast<DWORD>(charBuffer.size()),
        bufferCoord,
        &written
    );

    WriteConsoleOutputAttribute(
        hConsoleOutput,
        attrBuffer.data(),
        static_cast<DWORD>(attrBuffer.size()),
        bufferCoord,
        &written
    );

    SetConsoleCursorPosition(hConsoleOutput, bufferCoord);
}

// ----------------- Legacy Methods -----------------

void viewer::setConsoleWindowSize(int cols, int rows) {
    SMALL_RECT rect;
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = static_cast<SHORT>(cols - 1);
    rect.Bottom = static_cast<SHORT>(rows - 1);
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &rect);
}

void viewer::setConsoleBufferSize(int cols, int rows) {
    COORD size = {static_cast<SHORT>(cols), static_cast<SHORT>(rows)};
    SetConsoleScreenBufferSize(hConsoleOutput, size);
}

void viewer::clearConsole() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count, cellCount;
    COORD homeCoords = {0, 0};

    if (hConsoleOutput == INVALID_HANDLE_VALUE) return;

    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacterA(hConsoleOutput, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsoleOutput, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsoleOutput, homeCoords);
}

void viewer::DrawStringAt(int row, int col, const std::string& s) {
    drawString(col, row, s, Color::DEFAULT);
}

void viewer::DrawCharAt(int row, int col, char ch) {
    drawChar(col, row, ch, Color::DEFAULT);
}

void viewer::DrawBoxOnce() {
    drawBox(0, 0, width, height, '#', Color::CYAN);
    present();
}

std::vector<std::vector<char>> viewer::CreateBox() {
    return std::vector<std::vector<char>>(height, std::vector<char>(width, ' '));
}

void viewer::PrintImg(const std::vector<std::vector<char>>& img) {
    for (size_t r = 0; r < img.size() && static_cast<int>(r) < height; ++r) {
        for (size_t c = 0; c < img[r].size() && static_cast<int>(c) < width; ++c) {
            drawChar(static_cast<int>(c), static_cast<int>(r), img[r][c]);
        }
    }
    present();
}

std::pair<int, int> viewer::getRandomEmptyCell(const std::vector<std::vector<char>>& grid) {
    std::vector<std::pair<int, int>> emptyCells;
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == ' ') {
                emptyCells.push_back({static_cast<int>(i), static_cast<int>(j)});
            }
        }
    }
    if (emptyCells.empty()) return {-1, -1};

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(emptyCells.size()) - 1);
    return emptyCells[dist(gen)];
}

} // namespace emulator
