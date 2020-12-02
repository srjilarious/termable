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
    mBuffer.reserve(size.x*size.y);
}

vec2i 
termBuffer::size()
{
    return mSize;
}

void 
termBuffer::writeChar(
        vec2i pos, 
        char c, 
        color::basic fore, 
        color::basic back)
{
    // TODO: Implement.
}

void 
termBuffer::writeStr(
        vec2i pos, 
        std::string str, 
        color::basic fore, 
        color::basic back)
{
    // TODO: Implement.   
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

}
