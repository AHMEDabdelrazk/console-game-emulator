#include "qml_renderer.hpp"

namespace emulator {

qml_renderer::qml_renderer(int width, int height)
    : width(width), height(height),
      backBuffer(width * height),
      frontBuffer(width * height)
{
    clearBuffer();
    present();
}

void qml_renderer::clearScreen() {
    clearBuffer();
    present();
}

void qml_renderer::clearBuffer() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    ScreenCell blank{' ', static_cast<uint8_t>(Color::DEFAULT)};
    std::fill(backBuffer.begin(), backBuffer.end(), blank);
}

void qml_renderer::drawChar(int col, int row, char ch, Color color) {
    if (col < 0 || col >= width || row < 0 || row >= height) return;
    std::lock_guard<std::mutex> lock(bufferMutex);
    int idx = row * width + col;
    backBuffer[idx].ch = ch;
    backBuffer[idx].color = static_cast<uint8_t>(color);
}

void qml_renderer::drawString(int col, int row, const std::string& str, Color color) {
    if (row < 0 || row >= height) return;
    std::lock_guard<std::mutex> lock(bufferMutex);
    for (size_t i = 0; i < str.length(); ++i) {
        int c = col + static_cast<int>(i);
        if (c >= 0 && c < width) {
            int idx = row * width + c;
            backBuffer[idx].ch = str[i];
            backBuffer[idx].color = static_cast<uint8_t>(color);
        }
    }
}

void qml_renderer::drawBox(int startCol, int startRow, int boxWidth, int boxHeight, char border, Color color) {
    for (int r = startRow; r < startRow + boxHeight; ++r) {
        for (int c = startCol; c < startCol + boxWidth; ++c) {
            if (r == startRow || r == startRow + boxHeight - 1 ||
                c == startCol || c == startCol + boxWidth - 1) {
                drawChar(c, r, border, color);
            }
        }
    }
}

void qml_renderer::fillRect(int startCol, int startRow, int rectWidth, int rectHeight, char fill, Color color) {
    for (int r = startRow; r < startRow + rectHeight; ++r) {
        for (int c = startCol; c < startCol + rectWidth; ++c) {
            drawChar(c, r, fill, color);
        }
    }
}

void qml_renderer::present() {
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        frontBuffer = backBuffer;
    }
    if (onFramePresented) {
        onFramePresented();
    }
}

void qml_renderer::setFrameCallback(std::function<void()> callback) {
    onFramePresented = std::move(callback);
}

std::vector<ScreenCell> qml_renderer::getFrameSnapshot() const {
    std::lock_guard<std::mutex> lock(bufferMutex);
    return frontBuffer;
}

std::string qml_renderer::getPlainTextFrame() const {
    std::lock_guard<std::mutex> lock(bufferMutex);
    std::string text;
    text.reserve(width * height + height);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            text += frontBuffer[r * width + c].ch;
        }
        text += '\n';
    }
    return text;
}

} // namespace emulator
