#include <termable/termable.hpp>

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
    mBuffer[pos.y*mSize.x+pos.x].val = c;
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
    for(int ii = 0; ii < mBuffer.size(); ii++) {
        mBuffer[ii].val = c;// = {c, fore, back};
    }
    // for(auto& tc : mBuffer) {
    //     tc.val = c;
    //     tc.foregroundColor = fore;
    //     tc.backgroundColor = back; 
    // }
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


void 
termableLinux::renderBuffer(const termBuffer& buffer)
{
    // Move to start location
    setCursorPos({0,0});

    // Render each row of the buffer
    auto buffSize = buffer.size();
    for(int yy = 0; yy < buffSize.y; yy++) 
    {
        for(int xx = 0; xx < buffSize.x; xx++) 
        {
            // TODO: Add in color handling.

            const auto& tChar = buffer.buffer()[yy*buffSize.x+xx];
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

}

}
