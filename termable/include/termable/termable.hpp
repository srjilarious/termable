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
        Reset,
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
}


struct vec2i {
    int x, y;
};

using termColor = std::variant<color::basic, uint8_t>;
// using utf8Char = std::array<uint8_t, 4>;

struct utf8Char
{
    utf8Char();
    utf8Char(const utf8Char& other);
    utf8Char(const char u8Ch[]);
    utf8Char(std::initializer_list<int>u8Ch);
    utf8Char(char c);
    utf8Char(utf8Char&& other);
    ~utf8Char() = default;
    
    utf8Char& operator=(const utf8Char& other);
    utf8Char& operator=(utf8Char&& other);
    friend bool operator== (const utf8Char &c1, const utf8Char &c2);  
    friend bool operator!= (const utf8Char &c1, const utf8Char &c2);
    std::size_t numBytes;
    std::array<char, 4> values;

    // Counts the number of bytes in a UTF-8 code point.  Returns nullopt 
    // If the given pointer points to invalid UTF-8 data.
    std::optional<uint8_t> checkNumBytes(const char* ch);
    void write() const;
};

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

    void resize(vec2i size);

    vec2i size() const;
    const std::vector<termChar>& buffer() const;

    // Writes the a UTF-8 codepoint to the buffer at the given position.
    void writeChar(vec2i pos, 
                utf8Char c, 
                termColor fore = color::basic::Reset, 
                termColor back = color::basic::Reset);

    // Writes out a UTF-8 string into the buffer, returning the 
    // number of codepoints written out.
    int writeStr(vec2i pos, 
                std::string str, 
                termColor fore = color::basic::Reset, 
                termColor back = color::basic::Reset);

    void fill(utf8Char c,
                termColor fore = color::basic::Reset, 
                termColor back = color::basic::Reset);

    void horzLine(utf8Char c, vec2i start, int len,
            termColor fore = color::basic::Reset, 
            termColor back = color::basic::Reset);
    void vertLine(utf8Char c, vec2i start, int len,
            termColor fore = color::basic::Reset, 
            termColor back = color::basic::Reset);
};

enum ClearType : uint8_t {
    End = 0,
    Start = 1,
    All = 2
};

enum class BufferRenderOption {
    Origin,

    // Render the buffer into the console not moving the 
    Relative,

    // Don't move the console cursor at all before rendering.
    Rerender,
};

enum class NonAsciiChar
{
    Up,
    Down,
    Left,
    Right,
    Insert,
    Delete,
    PageUp,
    PageDown,
    Home,
    End
};

using KeyResult = std::variant<std::monostate, NonAsciiChar, char>;

class termable
{
public:
    virtual ~termable() = default;

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

    virtual void setBackgroundColor(termColor color) = 0;
    virtual void setForegroundColor(termColor color) = 0;

    // Gets a character from the keyboard (turns off buffering if needed)
    virtual KeyResult getch() = 0;

    virtual void showCursor(bool show) = 0;

    virtual void renderBuffer(const termBuffer& buffer, BufferRenderOption option) = 0;

    // Render a buffer skipping over parts that are the same as the old buffer.
    virtual void renderBuffer(const termBuffer& currBuffer, const termBuffer& oldBuffer) = 0;
};

namespace utf
{
    
};

class termableLinux : public termable
{
private:
    bool mCursorHidden = false;
    bool mBufferingEnabled = true;

    bool setInputBuffering(bool enabled);

public:
    ~termableLinux() override;

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

    void setBackgroundColor(termColor color) override;
    void setForegroundColor(termColor color) override;

    // Gets a character from the keyboard (turns off buffering if needed)
    KeyResult getch() override;

    void showCursor(bool show) override;

    void renderBuffer(const termBuffer& buffer, BufferRenderOption option) override;
    void renderBuffer(const termBuffer& currBuffer, const termBuffer& oldBuffer) override;
};

}
