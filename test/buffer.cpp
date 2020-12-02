// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>


int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();

    termable::termBuffer buffer({20,4});

    buffer.fill({0xc3, 0x97});

    buffer.writeChar({0, 0}, {'a'});
    buffer.writeChar({1, 0}, {'e'});
    buffer.writeChar({2, 0}, {'i'});
    buffer.writeChar({3, 0}, {'o'});
    buffer.writeChar({4, 0}, {'u'});

    // Hiragana a i u e o
    buffer.writeChar({0, 1}, {0xe3, 0x81, 0x82});
    buffer.writeChar({1, 1}, {0xe3, 0x81, 0x84});
    buffer.writeChar({2, 1}, {0xe3, 0x81, 0x86});
    buffer.writeChar({3, 1}, {0xe3, 0x81, 0x88});
    buffer.writeChar({4, 1}, {0xe3, 0x81, 0x8a});

    // U+1f600 - smiley face
    buffer.writeChar({0, 2}, {0xf0, 0x9f, 0x98, 0x80});
    
    term.renderBuffer(buffer);

    return 0;
}