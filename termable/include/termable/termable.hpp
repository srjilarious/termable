#pragma once

#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <memory>
#include <optional>

namespace termable
{

struct vec2i {
    int x, y;
};

class termable
{
public:
    virtual vec2i displaySize() const = 0;
    
    // void setCursorPos(vec2i pos) = 0;
    // void moveUp() = 0;
    // void moveDown() = 0;
    // void moveLeft() = 0;
    // void moveRight() = 0;

};

class termableLinux : public termable
{
public:
    vec2i displaySize() const override;
};

}
