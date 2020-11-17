#include <termable/termable.hpp>

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

namespace termable
{

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

}
