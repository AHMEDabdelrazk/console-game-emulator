#pragma once

#include "types.hpp"

namespace emulator {

class IInputListener {
public:
    virtual ~IInputListener() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual InputKey getKey() = 0;
    virtual bool hasKey() const = 0;
    virtual void clearKeys() = 0;
};

} // namespace emulator
