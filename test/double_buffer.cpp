// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();

    termable::termBuffer buffer1({20,5}), buffer2({20,5});
    buffer1.fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Blue);
    buffer2.fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Blue);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;

    std::vector<termable::vec2i> stars;
    stars.push_back({1, 0});
    stars.push_back({13, 0});
    stars.push_back({4, 1});
    stars.push_back({19, 1});
    stars.push_back({8, 2});
    stars.push_back({15, 2});
    stars.push_back({4, 3});
    stars.push_back({16, 4});

    for(int cnt = 0; cnt < 1000; cnt++) {

        currBuffer->fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Blue);
        for(auto& s : stars) {
            s.x++;
            if(s.x > 19) {
                s.x = 0;
            }

            currBuffer->writeChar(s, {'*'});
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