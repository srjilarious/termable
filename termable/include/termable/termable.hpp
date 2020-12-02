#pragma once

#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <memory>
#include <optional>
#include <variant>

namespace termable
{

namespace color
{
    enum class basic 
    {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,

        Gray,
        BoldRed,
        BoldGreen,
        BoldYellow,
        BoldBlue,
        BoldMagenta,
        BoldCyan,
        BoldWhite
    };

    // TODO: Move to linux specific impl.
    constexpr const char ResetColor[]   = "\033[0m";
    namespace foreground
    {
        constexpr const char RedColor[]     = "\033[31m";
        constexpr const char GreenColor[]   = "\033[32m";
        constexpr const char YellowColor[]  = "\033[33m";
        constexpr const char BlueColor[]    = "\033[34m";
        constexpr const char MagentaColor[] = "\033[35m";
        constexpr const char CyanColor[]    = "\033[36m";
        constexpr const char WhiteColor[]   = "\033[37m";

        constexpr const char GrayColor[]       = "\033[90m";
        constexpr const char BoldRedColor[]    = "\033[91m";
        constexpr const char BoldGreenColor[]  = "\033[92m";
        constexpr const char BoldYellowColor[] = "\033[93m";
        constexpr const char BoldBlueColor[]   = "\033[94m";
        constexpr const char BoldMagentaColor[]= "\033[95m";
        constexpr const char BoldCyanColor[]   = "\033[96m";
        constexpr const char BoldWhiteColor[]  = "\033[97m";

        std::string color256(uint8_t which);
    }

    namespace background
    {
        constexpr const char RedColor[]     = "\033[41m";
        constexpr const char GreenColor[]   = "\033[42m";
        constexpr const char YellowColor[]  = "\033[43m";
        constexpr const char BlueColor[]    = "\033[44m";
        constexpr const char MagentaColor[] = "\033[45m";
        constexpr const char CyanColor[]    = "\033[46m";
        constexpr const char WhiteColor[]   = "\033[47m";

        constexpr const char GrayColor[]       = "\033[40;1m";
        constexpr const char BoldRedColor[]    = "\033[41;1m";
        constexpr const char BoldGreenColor[]  = "\033[42;1m";
        constexpr const char BoldYellowColor[] = "\033[43;1m";
        constexpr const char BoldBlueColor[]   = "\033[44;1m";
        constexpr const char BoldMagentaColor[]= "\033[45;1m";
        constexpr const char BoldCyanColor[]   = "\033[46;1m";
        constexpr const char BoldWhiteColor[]  = "\033[47;1m";

        std::string color256(uint8_t which);
    }
}


struct vec2i {
    int x, y;
};

using termColor = std::variant<color::basic, uint8_t>;
using utf8Char = std::array<uint8_t, 4>;

struct termChar
{
    // UTF-8 can be 1-4 bytes.
    utf8Char val;
    termColor foregroundColor, backgroundColor;
};

class termBuffer {
private:
    std::vector<termChar> mBuffer;
    vec2i mSize;

public:
    termBuffer(vec2i size);

    vec2i size() const;
    const std::vector<termChar>& buffer() const;

    void writeChar(vec2i pos, 
                utf8Char c, 
                termColor fore = color::basic::White, 
                termColor back = color::basic::Black);

    void writeStr(vec2i pos, 
                std::string str, 
                termColor fore = color::basic::White, 
                termColor back = color::basic::Black);
};

enum ClearType : uint8_t {
    End = 0,
    Start = 1,
    All = 2
};

class termable
{
public:
    virtual vec2i displaySize() const = 0;
    
    virtual void setCursorPos(vec2i pos) = 0;
    virtual void moveUp(uint32_t amount = 1) = 0;
    virtual void moveDown(uint32_t amount = 1) = 0;
    virtual void moveLeft(uint32_t amount = 1) = 0;
    virtual void moveRight(uint32_t amount = 1) = 0;

    virtual void moveUpLine(uint32_t amount = 1) = 0;
    virtual void moveDownLine(uint32_t amount = 1) = 0;

    virtual void clear(ClearType type = ClearType::All) = 0;
    virtual void clearLine(ClearType type = ClearType::All) = 0;

    virtual void renderBuffer(const termBuffer& buffer) = 0;
};

class termableLinux : public termable
{
public:
    vec2i displaySize() const override;

    void setCursorPos(vec2i pos) override;
    void moveUp(uint32_t amount = 1) override;
    void moveDown(uint32_t amount = 1) override;
    void moveLeft(uint32_t amount = 1) override;
    void moveRight(uint32_t amount = 1) override;

    void moveUpLine(uint32_t amount = 1) override;
    void moveDownLine(uint32_t amount = 1) override;

    void clear(ClearType type = ClearType::All) override;
    void clearLine(ClearType type = ClearType::All) override;

    void renderBuffer(const termBuffer& buffer) override;
};

}
