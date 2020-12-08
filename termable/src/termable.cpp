#include <termable/termable.hpp>

#include <variant>
#include <optional>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

namespace termable
{

namespace color
{
namespace foreground
{
std::string color256(uint8_t which) {
    return std::string("\u001b[38;5;") + std::to_string(which) + "m";
}
}

namespace background
{
std::string color256(uint8_t which) {
    return std::string("\u001b[48;5;") + std::to_string(which) + "m";
}
}

}


namespace utf
{

std::optional<uint8_t> 
numBytesInUtf8Char(const uint8_t* ch)
{
    // Check for single byte UTF-8
    if((*ch & 0x80) == 0x00) {
        return std::optional<uint8_t>(1);
    }
    // 2-byte code point
    else if((*ch & 0xe0) == 0xc0) {
        return std::optional<uint8_t>(2);
    }
    // 3-byte code point
    else if((*ch & 0xf0) == 0xe0) {
        return std::optional<uint8_t>(3);
    }
    // 4-byte code point
    else if((*ch & 0xf8) == 0xf0) {
        return std::optional<uint8_t>(4);
    }
    // Incorrect codepoint
    else {
        return std::nullopt;
    }
}

}
termBuffer::termBuffer(vec2i size) :
    mSize(size)
{
    mBuffer.resize(size.x*size.y);
}

vec2i 
termBuffer::size() const
{
    return mSize;
}

const std::vector<termChar>& 
termBuffer::buffer() const
{
    return mBuffer;
}

void 
termBuffer::writeChar(
        vec2i pos, 
        utf8Char c, 
        termColor fore, 
        termColor back)
{
    mBuffer[pos.y*mSize.x+pos.x] = {c, fore, back};
}

int
termBuffer::writeStr(
        vec2i pos, 
        std::string str, 
        termColor fore, 
        termColor back)
{
    int written = 0;
    const uint8_t* ch = reinterpret_cast<const uint8_t*>(str.data());
    auto bufferOffset = pos.y*mSize.x+pos.x;
    while(*ch != '\0') {
        auto numBytesOpt = utf::numBytesInUtf8Char(ch);
        if(numBytesOpt.has_value()) {
            auto numBytes = numBytesOpt.value();
            if(numBytes == 1) {
                mBuffer[bufferOffset] = {{ch[0], 0, 0, 0}, fore, back};
            }
            else if(numBytes == 2) {
                mBuffer[bufferOffset] = {{ch[0], ch[1], 0, 0}, fore, back};
            }
            else if(numBytes == 3) {
                mBuffer[bufferOffset] = {{ch[0], ch[1], ch[2], 0}, fore, back};
            }
            else if(numBytes == 4) {
                mBuffer[bufferOffset] = {{ch[0], ch[1], ch[2], ch[3]}, fore, back};
            }
            else {
                // Error, so bail out.
                return written;
            }

            ch+= numBytes;
            bufferOffset++;
            written++;
        }
        else {
            // Hit invalid codepoint, so exit.
            return written;
        }
    }

    return written;
}


void 
termBuffer::fill(
        utf8Char c,
        termColor fore, 
        termColor back)
{
    for(auto& tc : mBuffer) {
        tc = {c, fore, back};
    }
}

vec2i 
termableLinux::displaySize() const
{
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    vec2i sx;
    sx.x = size.ws_col;
    sx.y = size.ws_row;
    return sx;
}

void
termableLinux::setCursorPos(vec2i pos)
{
    // +1 accounts for coords being 1-based rather than our 0, based coords.
    printf("\u001b[%u;%uH", pos.x+1, pos.y+1);
}

void 
termableLinux::moveUp(uint32_t amount) 
{
    printf("\u001b[%uA", amount);
}

void 
termableLinux::moveDown(uint32_t amount) 
{
    printf("\u001b[%uB", amount);
}

void 
termableLinux::moveLeft(uint32_t amount) 
{
    printf("\u001b[%uD", amount);
}

void 
termableLinux::moveRight(uint32_t amount)
{
    printf("\u001b[%uC", amount);
}

void 
termableLinux::moveUpLine(uint32_t amount)
{
    printf("\u001b[%uE", amount);
}

void 
termableLinux::moveDownLine(uint32_t amount)
{
    printf("\u001b[%uF", amount);
}

void 
termableLinux::clear(ClearType type)
{
    printf("\u001b[%uJ", static_cast<uint8_t>(type));
}

void 
termableLinux::clearLine(ClearType type)
{
    printf("\u001b[%uK", static_cast<uint8_t>(type));
}

void termableLinux::setBackgroundColor(termColor color)
{
    if(std::holds_alternative<color::basic>(color)) {
        color::basic basicBg = std::get<color::basic>(color);
        switch (basicBg)
        {
            case color::basic::Reset:
                printf(color::ResetColor);
                break;
            case color::basic::Black:
                printf(color::background::BlackColor);
                break;
            case color::basic::Red:
                printf(color::background::RedColor);
                break;
            case color::basic::Green:
                printf(color::background::GreenColor);
                break;
            case color::basic::Yellow:
                printf(color::background::YellowColor);
                break;
            case color::basic::Blue:
                printf(color::background::BlueColor);
                break;
            case color::basic::Magenta:
                printf(color::background::MagentaColor);
                break;
            case color::basic::Cyan:
                printf(color::background::CyanColor);
                break;
            case color::basic::White:
                printf(color::background::WhiteColor);
                break;
            case color::basic::BoldRed:
                printf(color::background::BoldRedColor);
                break;
            case color::basic::BoldGreen:
                printf(color::background::BoldGreenColor);
                break;
            case color::basic::BoldYellow:
                printf(color::background::BoldYellowColor);
                break;
            case color::basic::BoldBlue:
                printf(color::background::BoldBlueColor);
                break;
            case color::basic::BoldMagenta:
                printf(color::background::BoldMagentaColor);
                break;
            case color::basic::BoldCyan:
                printf(color::background::BoldCyanColor);
                break;
            case color::basic::BoldWhite:
                printf(color::background::BoldWhiteColor);
                break;
        }
    }
    else {
        printf("%s", color::background::color256(std::get<uint8_t>(color)).c_str());
    }
}

void termableLinux::setForegroundColor(termColor color)
{
    if(std::holds_alternative<color::basic>(color)) {
        color::basic basicFg = std::get<color::basic>(color);
        switch (basicFg)
        {
            case color::basic::Reset:
                printf(color::ResetColor);
                break;
            case color::basic::Black:
                printf(color::foreground::BlackColor);
                break;
            case color::basic::Red:
                printf(color::foreground::RedColor);
                break;
            case color::basic::Green:
                printf(color::foreground::GreenColor);
                break;
            case color::basic::Yellow:
                printf(color::foreground::YellowColor);
                break;
            case color::basic::Blue:
                printf(color::foreground::BlueColor);
                break;
            case color::basic::Magenta:
                printf(color::foreground::MagentaColor);
                break;
            case color::basic::Cyan:
                printf(color::foreground::CyanColor);
                break;
            case color::basic::White:
                printf(color::foreground::WhiteColor);
                break;
            case color::basic::BoldRed:
                printf(color::foreground::BoldRedColor);
                break;
            case color::basic::BoldGreen:
                printf(color::foreground::BoldGreenColor);
                break;
            case color::basic::BoldYellow:
                printf(color::foreground::BoldYellowColor);
                break;
            case color::basic::BoldBlue:
                printf(color::foreground::BoldBlueColor);
                break;
            case color::basic::BoldMagenta:
                printf(color::foreground::BoldMagentaColor);
                break;
            case color::basic::BoldCyan:
                printf(color::foreground::BoldCyanColor);
                break;
            case color::basic::BoldWhite:
                printf(color::foreground::BoldWhiteColor);
                break;
        }
    }
    else {
        printf("%s", color::foreground::color256(std::get<uint8_t>(color)).c_str());
    }
}


void 
termableLinux::renderBuffer(const termBuffer& buffer)
{
    // Move to start location
    setCursorPos({0,0});

    // Rest color
    printf(color::ResetColor);

    termColor fore = color::basic::White;
    termColor back = color::basic::Black;

    // Render each row of the buffer
    auto buffSize = buffer.size();
    for(int yy = 0; yy < buffSize.y; yy++) 
    {
        for(int xx = 0; xx < buffSize.x; xx++) 
        {
            const auto& tChar = buffer.buffer()[yy*buffSize.x+xx];

            // Check color and change if needed
            setBackgroundColor(tChar.backgroundColor);
            setForegroundColor(tChar.foregroundColor);

            // Write out character
            const uint8_t* valPtr = static_cast<const uint8_t*>(tChar.val.data());
            auto utfByteCountOpt = utf::numBytesInUtf8Char(valPtr);

            if(utfByteCountOpt.has_value())
            {
                auto byteCount = utfByteCountOpt.value();
                if(byteCount == 1) {
                    printf("%c", valPtr[0]);
                }
                else if(byteCount == 2) {
                    printf("%c%c", valPtr[0], valPtr[1]);
                }
                else if(byteCount == 3) {
                    printf("%c%c%c", valPtr[0], valPtr[1], valPtr[2]);
                }
                else if(byteCount == 4) {
                    printf("%c%c%c%c", valPtr[0], valPtr[1], valPtr[2], valPtr[3]);
                }
            }
            else 
            {
                // Incorrect codepoint - Print the question mark in a diamond char.
                printf(u8"\ufffd");
            }
        }

        printf("\n");
    }

    // Rest color
    printf(color::ResetColor);
}

}
