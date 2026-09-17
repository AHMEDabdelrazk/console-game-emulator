#pragma once

#include <vector>
#include <string>
#include <functional>
#include <mutex>
#include "types.hpp"
#include "renderer_interface.hpp"

namespace emulator {

struct ScreenCell {
    char ch = ' ';
    uint8_t color = static_cast<uint8_t>(Color::DEFAULT);
};

class qml_renderer : public IRenderer {
public:
    qml_renderer(int width = 100, int height = 28);
    ~qml_renderer() override = default;

    void clearScreen() override;
    void clearBuffer() override;
    void drawChar(int col, int row, char ch, Color color = Color::DEFAULT) override;
    void drawString(int col, int row, const std::string& str, Color color = Color::DEFAULT) override;
    void drawBox(int startCol, int startRow, int width, int height, char border = '#', Color color = Color::DEFAULT) override;
    void fillRect(int startCol, int startRow, int width, int height, char fill = ' ', Color color = Color::DEFAULT) override;
    void present() override;
    int getWidth() const override { return width; }
    int getHeight() const override { return height; }
    void setCursorVisible(bool) override {}

    // Callback for when a frame is presented
    void setFrameCallback(std::function<void()> callback);

    // Frame buffer snapshot for QML UI
    std::vector<ScreenCell> getFrameSnapshot() const;
    std::string getPlainTextFrame() const;

private:
    int width;
    int height;
    mutable std::mutex bufferMutex;
    std::vector<ScreenCell> backBuffer;
    std::vector<ScreenCell> frontBuffer;
    std::function<void()> onFramePresented;
};

using QmlRenderer = qml_renderer;

} // namespace emulator
