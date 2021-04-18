#include <termable/termable.hpp>

#include <variant>
#include <optional>

#include <signal.h>
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#include <termios.h>

namespace termable
{
namespace lin 
{
    constexpr const char HideCursor[]       = "\033[?25l";
    constexpr const char ShowCursor[]       = "\033[?25h";

namespace color
{
    constexpr const char ResetColor[]   = "\033[0m";

namespace foreground
{
    constexpr const char BlackColor[]     = "\033[30m";
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

    std::string color256(uint8_t which) {
        return std::string("\u001b[38;5;") + std::to_string(which) + "m";
    }
}

namespace background
{
    constexpr const char BlackColor[]     = "\033[40m";
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

    std::string color256(uint8_t which) {
        return std::string("\u001b[48;5;") + std::to_string(which) + "m";
    }
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

void 
writeUtf8Char(const uint8_t* ch)
{
    auto utfByteCountOpt = utf::numBytesInUtf8Char(ch);

    if(utfByteCountOpt.has_value())
    {
        auto byteCount = utfByteCountOpt.value();
        if(byteCount == 1) {
            printf("%c", ch[0]);
        }
        else if(byteCount == 2) {
            printf("%c%c", ch[0], ch[1]);
        }
        else if(byteCount == 3) {
            printf("%c%c%c", ch[0], ch[1], ch[2]);
        }
        else if(byteCount == 4) {
            printf("%c%c%c%c", ch[0], ch[1], ch[2], ch[3]);
        }
    }
    else 
    {
        // Incorrect codepoint - Print the question mark in a diamond char.
        printf(u8"\ufffd");
    }
}

}

termBuffer::termBuffer(vec2i size) :
    mSize(size)
{
    mBuffer.resize(size.x*size.y);
}

void 
termBuffer::resize(vec2i size)
{
    auto oldBuffer = mBuffer;
    mBuffer.resize(size.x*size.y);

    std::size_t oldBuffIdx = 0;
    std::size_t bufferIdx = 0;

    for(int yy = 0; yy < mSize.y; yy++) {

        for(int xx = 0; xx < mSize.x; xx++) {
            mBuffer[bufferIdx] = oldBuffer[oldBuffIdx];
            bufferIdx++;
            oldBuffIdx++;
        }

        for(int xx = 0; xx < (size.x - mSize.x); xx++) {
            mBuffer[bufferIdx++] = {};
        }
    }

    for(int yy = 0; yy < (size.y - mSize.y); yy++) {
        for(int xx = 0; xx < mSize.x; xx++) {
            mBuffer[bufferIdx++] = {};
        }
    }
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

termableLinux::~termableLinux()
{
    if(!mBufferingEnabled) {
        setInputBuffering(true);
    }
}

void
termableLinux::setCursorPos(vec2i pos)
{
    // +1 accounts for coords being 1-based rather than our 0, based coords.
    printf("\u001b[%u;%uH", pos.y+1, pos.x+1);
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
                printf(lin::color::ResetColor);
                break;
            case color::basic::Black:
                printf(lin::color::background::BlackColor);
                break;
            case color::basic::Red:
                printf(lin::color::background::RedColor);
                break;
            case color::basic::Green:
                printf(lin::color::background::GreenColor);
                break;
            case color::basic::Yellow:
                printf(lin::color::background::YellowColor);
                break;
            case color::basic::Blue:
                printf(lin::color::background::BlueColor);
                break;
            case color::basic::Magenta:
                printf(lin::color::background::MagentaColor);
                break;
            case color::basic::Cyan:
                printf(lin::color::background::CyanColor);
                break;
            case color::basic::White:
                printf(lin::color::background::WhiteColor);
                break;
            case color::basic::BoldRed:
                printf(lin::color::background::BoldRedColor);
                break;
            case color::basic::BoldGreen:
                printf(lin::color::background::BoldGreenColor);
                break;
            case color::basic::BoldYellow:
                printf(lin::color::background::BoldYellowColor);
                break;
            case color::basic::BoldBlue:
                printf(lin::color::background::BoldBlueColor);
                break;
            case color::basic::BoldMagenta:
                printf(lin::color::background::BoldMagentaColor);
                break;
            case color::basic::BoldCyan:
                printf(lin::color::background::BoldCyanColor);
                break;
            case color::basic::BoldWhite:
                printf(lin::color::background::BoldWhiteColor);
                break;
        }
    }
    else {
        printf("%s", lin::color::background::color256(std::get<uint8_t>(color)).c_str());
    }
}

void termableLinux::setForegroundColor(termColor color)
{
    if(std::holds_alternative<color::basic>(color)) {
        color::basic basicFg = std::get<color::basic>(color);
        switch (basicFg)
        {
            case color::basic::Reset:
                printf(lin::color::ResetColor);
                break;
            case color::basic::Black:
                printf(lin::color::foreground::BlackColor);
                break;
            case color::basic::Red:
                printf(lin::color::foreground::RedColor);
                break;
            case color::basic::Green:
                printf(lin::color::foreground::GreenColor);
                break;
            case color::basic::Yellow:
                printf(lin::color::foreground::YellowColor);
                break;
            case color::basic::Blue:
                printf(lin::color::foreground::BlueColor);
                break;
            case color::basic::Magenta:
                printf(lin::color::foreground::MagentaColor);
                break;
            case color::basic::Cyan:
                printf(lin::color::foreground::CyanColor);
                break;
            case color::basic::White:
                printf(lin::color::foreground::WhiteColor);
                break;
            case color::basic::BoldRed:
                printf(lin::color::foreground::BoldRedColor);
                break;
            case color::basic::BoldGreen:
                printf(lin::color::foreground::BoldGreenColor);
                break;
            case color::basic::BoldYellow:
                printf(lin::color::foreground::BoldYellowColor);
                break;
            case color::basic::BoldBlue:
                printf(lin::color::foreground::BoldBlueColor);
                break;
            case color::basic::BoldMagenta:
                printf(lin::color::foreground::BoldMagentaColor);
                break;
            case color::basic::BoldCyan:
                printf(lin::color::foreground::BoldCyanColor);
                break;
            case color::basic::BoldWhite:
                printf(lin::color::foreground::BoldWhiteColor);
                break;
        }
    }
    else {
        printf("%s", lin::color::foreground::color256(std::get<uint8_t>(color)).c_str());
    }
}

bool 
termableLinux::setInputBuffering(bool enabled)
{
    termios tConf = {0};
    if(tcgetattr(0, &tConf) < 0) {
        return false;
    }

    if(enabled) {
        tConf.c_iflag |= (IGNBRK | BRKINT | PARMRK | ISTRIP
                        | INLCR | IXON);
        //tConf.c_iflag &= ~(IGNCR | ICRNL);
        tConf.c_oflag |= OPOST;
        tConf.c_lflag |= (ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        tConf.c_cflag |= (CSIZE | PARENB);
        tConf.c_cflag &= ~CS8;
        // tConf.c_lflag |= ICANON;
        // tConf.c_lflag |= ECHO;
    }
    // Disable echo and canonical terminal mode.
    else {
        tConf.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                        | INLCR | IXON);
        //tConf.c_iflag |= (IGNCR | ICRNL);
        tConf.c_oflag &= ~OPOST;
        tConf.c_lflag &= ~(ECHO | ECHONL | ECHOCTL | ICANON | ISIG | IEXTEN);
        tConf.c_cflag &= ~(CSIZE | PARENB);
        tConf.c_cflag |= CS8;
        // tConf.c_lflag &= ~ICANON;
        // tConf.c_lflag &= ~ECHO;
        tConf.c_cc[VMIN] = 0;
        tConf.c_cc[VTIME] = 0;
    }

    if (tcsetattr(0, TCSADRAIN, &tConf) < 0) {
        return false;
    }

    mBufferingEnabled = enabled;
    return true;
}

KeyResult
termableLinux::getch() 
{
    if(mBufferingEnabled) {
        setInputBuffering(false);
    }

    KeyResult res = std::monostate{};
    char ch[8] = {0};
    if (read(0, &ch, 8) == 1) {
        res = static_cast<char>(ch[0]);
    }
    else {
        if(ch[0] == 27 && ch[1] == 91 && ch[2] == 65) {
            res = NonAsciiChar::Up;
        }
        else if(ch[0] == 27 && ch[1] == 91 && ch[2] == 67) {
            res = NonAsciiChar::Right;
        }
        else if(ch[0] == 27 && ch[1] == 91 && ch[2] == 68) {
            res = NonAsciiChar::Left;
        }
        else if(ch[0] == 27 && ch[1] == 91 && ch[2] == 66) {
            res = NonAsciiChar::Down;
        }
    }

    return res;
}

void 
termableLinux::showCursor(bool show)
{
    if(show) {
        printf(lin::ShowCursor);
        mCursorHidden = false;
    }
    else {
        printf(lin::HideCursor);
        mCursorHidden = true;
    }
}

void 
termableLinux::renderBuffer(const termBuffer& buffer, BufferRenderOption option)
{
    if(option == BufferRenderOption::Origin) {
        // Move to start location
        setCursorPos({0,0});
    }
    else if(option == BufferRenderOption::Relative) {
        moveUp(buffer.size().y);
    }

    // Rest color
    printf(lin::color::ResetColor);

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
            utf::writeUtf8Char(tChar.val.data());
        }

        printf("\n");
    }

    // Rest color
    printf(lin::color::ResetColor);
}


void 
termableLinux::renderBuffer(
        const termBuffer& currBuffer, 
        const termBuffer& oldBuffer
    )
{
    auto oldSize = oldBuffer.size();

    // Render each row of the buffer
    auto buffSize = currBuffer.size();
    size_t buffAddr = 0;
    for(int yy = 0; yy < buffSize.y; yy++) 
    {
        int lastX = -1;
        for(int xx = 0; xx < buffSize.x; xx++) {
            const auto& oldCh = oldBuffer.buffer()[buffAddr];
            const auto& newCh = currBuffer.buffer()[buffAddr];
            if(oldCh.val != newCh.val /*||
               oldCh.backgroundColor != newCh.backgroundColor ||
               oldCh.foregroundColor != newCh.foregroundColor*/) {
                if(lastX != xx) {
                    setCursorPos({xx, yy});
                    lastX = xx;
                }
                
                // Check color and change if needed
                setBackgroundColor(newCh.backgroundColor);
                setForegroundColor(newCh.foregroundColor);
                utf::writeUtf8Char(newCh.val.data());
            }

            buffAddr++;
        }

        //buffAddr++;
    }

    setCursorPos(currBuffer.size());
    setBackgroundColor( color::basic::Reset);
    setForegroundColor( color::basic::Reset);
}

}
