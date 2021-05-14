// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>


int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();
    term.showCursor(false);

    auto sz = term.displaySize();    
    termable::termBuffer buffer1(sz), buffer2(sz);
    buffer1.fill({' '}, termable::color::basic::BoldWhite, termable::color::basic::Black);
    buffer2.fill({'.'}, termable::color::basic::BoldWhite, termable::color::basic::Black);

    term.eventHandler = [&] (termable::TermableEvent e) {
        if(e == termable::TermableEvent::Resized) {
            sz = term.displaySize();
            buffer1.resize(sz);
            buffer2.resize(sz);
        }
    };
    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
    termable::vec2i pos = {sz.x/2,sz.y/2};
    termable::KeyResult c = std::monostate{};
    while(true) {
        c = term.getch();
        if(std::holds_alternative<char>(c)) {
            auto ch = std::get<char>(c);
            if(ch == 'q') {
                break;
            }
        }
        else if(std::holds_alternative<termable::NonAsciiChar>(c)) {
            auto nac = std::get<termable::NonAsciiChar>(c);

            switch(nac) {
                case termable::NonAsciiChar::Up:
                    pos.y--;
                    if(pos.y <= 0) pos.y = 0;
                    break;
                case termable::NonAsciiChar::Left:
                    pos.x--;
                    if(pos.x <= 0) pos.x = 0;
                    break;
                case termable::NonAsciiChar::Down:
                    pos.y++;
                    if(pos.y >= sz.y-1) pos.y = sz.y-1;
                    break;
                case termable::NonAsciiChar::Right:
                    pos.x++;
                    if(pos.x >= sz.x-1) pos.x = sz.x-1;
                    break;
            }
            
            currBuffer->fill({' '});
            currBuffer->writeChar(pos, {'*'});
            term.renderBuffer(*currBuffer, *oldBuffer);
            // Swap the buffers.
            std::swap(currBuffer, oldBuffer);
        
            fflush(stdout);
        }

    }
    term.showCursor(true);
    return 0;
}
