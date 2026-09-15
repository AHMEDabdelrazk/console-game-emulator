#pragma once

#include <string>
#include "types.hpp"

namespace emulator {

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void clearScreen() = 0;
    virtual void clearBuffer() = 0;
    virtual void drawChar(int col, int row, char ch, Color color = Color::DEFAULT) = 0;
    virtual void drawString(int col, int row, const std::string& str, Color color = Color::DEFAULT) = 0;
    virtual void drawBox(int startCol, int startRow, int width, int height, char border = '#', Color color = Color::DEFAULT) = 0;
    virtual void fillRect(int startCol, int startRow, int width, int height, char fill = ' ', Color color = Color::DEFAULT) = 0;
    virtual void present() = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual void setCursorVisible(bool visible) = 0;
};

} // namespace emulator
