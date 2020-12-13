// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>
#include <random>

int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();

    auto sz = term.displaySize();    
    termable::termBuffer buffer1(sz), buffer2(sz);
    buffer1.fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Black);
    buffer2.fill({'.'}, termable::color::basic::BoldWhite, termable::color::basic::Black);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;

    std::default_random_engine rand;
    std::uniform_int_distribution<int> dist_x(0, sz.x-1), dist_y(0, sz.y-1);

    std::vector<termable::vec2i> stars;
    for(int ii = 0; ii < 100; ii++) {
        stars.push_back({dist_x(rand), dist_y(rand)});
    }

    for(int cnt = 0; cnt < 1000; cnt++) {

        currBuffer->fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Black);
        for(auto& s : stars) {
            s.x++;
            if(s.x >= sz.x) {
                s.x = 0;
            }

            currBuffer->writeChar(s, {'*'}, termable::color::basic::BoldYellow, termable::color::basic::Black);
        }

        term.setCursorPos({0,0});
        term.renderBuffer(*currBuffer, *oldBuffer);

        // Swap the buffers.
        std::swap(currBuffer, oldBuffer);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    return 0;
}