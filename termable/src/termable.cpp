#include <termable/termable.hpp>

#include <variant>
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

void 
termBuffer::writeStr(
        vec2i pos, 
        std::string str, 
        termColor fore, 
        termColor back)
{
    // TODO: Implement.   
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
            //if(tChar.backgroundColor != back) {
            //    back = tChar.backgroundColor;
                setBackgroundColor(tChar.backgroundColor);
            //}

            //if(tChar.foregroundColor != fore) {
            //    fore = tChar.foregroundColor;
                setForegroundColor(tChar.foregroundColor);
            //}

            // Write out character
            const uint8_t* valPtr = tChar.val.data();

            // Check for single byte UTF-8
            if((*valPtr & 0x80) == 0x00) {
                printf("%c", valPtr[0]);
            }
            // 2-byte code point
            else if((*valPtr & 0xe0) == 0xc0) {
                printf("%c%c", valPtr[0], valPtr[1]);
            }
            // 3-byte code point
            else if((*valPtr & 0xf0) == 0xe0) {
                printf("%c%c%c", valPtr[0], valPtr[1], valPtr[2]);
            }
            // 4-byte code point
            else if((*valPtr & 0xf8) == 0xf0) {
                printf("%c%c%c%c", valPtr[0], valPtr[1], valPtr[2], valPtr[3]);
            }
            // Incorrect codepoint
            else {
                // Print the question mark in a diamond char.
                printf(u8"\ufffd");
                //printf("val: %x %x %x %x\n", valPtr[0], valPtr[1], valPtr[2], valPtr[3]);
            }
            
        }
        printf("\n");
    }

    // Rest color
    printf(color::ResetColor);
}

}
