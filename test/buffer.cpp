// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>

using namespace termable;

int main(int argc, char** argv)
{
    termableLinux term;
    term.clear();

    termBuffer buffer({20,5});

    buffer.fill(utf8Char({0xc3, 0x97}), color::basic::BoldRed, color::basic::Reset);

    buffer.writeChar({0, 0}, 'a', color::basic::BoldRed);
    buffer.writeChar({1, 0}, 'e', color::basic::BoldYellow);
    buffer.writeChar({2, 0}, 'i', color::basic::BoldBlue);
    buffer.writeChar({3, 0}, 'o', color::basic::BoldGreen);
    buffer.writeChar({4, 0}, 'u', color::basic::BoldMagenta);

    // Hiragana a i u e o
    auto chs = {0xe3, 0x81, 0x82};
    buffer.writeChar({0, 1}, utf8Char({0xe3, 0x81, 0x82}), termable::color::basic::BoldYellow, termable::color::basic::Green);
    buffer.writeChar({1, 1}, utf8Char({0xe3, 0x81, 0x84}), termable::color::basic::Black, termable::color::basic::White);
    buffer.writeChar({2, 1}, utf8Char({0xe3, 0x81, 0x86}));
    buffer.writeChar({3, 1}, utf8Char({0xe3, 0x81, 0x88}), termable::color::basic::BoldYellow, termable::color::basic::Green);
    buffer.writeChar({4, 1}, utf8Char({0xe3, 0x81, 0x8a}));

    // U+1f600 - smiley face
    buffer.writeChar({0, 2}, utf8Char({0xf0, 0x9f, 0x98, 0x80}), termable::color::basic::BoldYellow, termable::color::basic::Black);
    
    buffer.writeStr({2, 3}, u8"Grüss Gott!", termable::color::basic::BoldYellow, termable::color::basic::Blue);
    buffer.writeStr({1,4}, u8"\u00e4\u2308\u2309\u231b");
    term.renderBuffer(buffer, termable::BufferRenderOption::Origin);

    return 0;
}