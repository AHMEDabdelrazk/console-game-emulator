#pragma once

#include <vector>
#include <string>
#include <windows.h>
#include "types.hpp"
#include "renderer_interface.hpp"

namespace emulator {

class viewer : public IRenderer {
public:
    explicit viewer(int width = 100, int height = 28);
    ~viewer() override;

    // IRenderer implementation
    void clearScreen() override;
    void clearBuffer() override;
    void drawChar(int col, int row, char ch, Color color = Color::DEFAULT) override;
    void drawString(int col, int row, const std::string& str, Color color = Color::DEFAULT) override;
    void drawBox(int startCol, int startRow, int width, int height, char border = '#', Color color = Color::DEFAULT) override;
    void fillRect(int startCol, int startRow, int width, int height, char fill = ' ', Color color = Color::DEFAULT) override;
    void present() override;
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }
    void setCursorVisible(bool visible) override;

    // Legacy / Console Setup Helpers
    void setConsoleWindowSize(int cols, int rows);
    void setConsoleBufferSize(int cols, int rows);
    void clearConsole();
    void DrawStringAt(int row, int col, const std::string& s);
    void DrawCharAt(int row, int col, char ch);
    void DrawBoxOnce();
    std::vector<std::vector<char>> CreateBox();
    void PrintImg(const std::vector<std::vector<char>>& img);
    std::pair<int, int> getRandomEmptyCell(const std::vector<std::vector<char>>& grid);

private:
    int width;
    int height;
    HANDLE hConsoleOutput;
    std::vector<char> charBuffer;
    std::vector<WORD> attrBuffer;

    void initializeConsole();
    WORD toConsoleAttribute(Color color);
};

using ConsoleViewer = viewer;

} // namespace emulator
