// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>
#include <algorithm>

void writeStr(termable::termBuffer *buff, termable::vec2i pos, std::string value, bool selected)
{
    if(selected)
    {
        buff->writeStr(pos, "[", termable::color::basic::BoldYellow, termable::color::basic::Blue);
        pos.x++;
        buff->writeStr(pos, "*", termable::color::basic::BoldCyan, termable::color::basic::Blue);
        pos.x++;
        buff->writeStr(pos, "] ", termable::color::basic::BoldYellow, termable::color::basic::Blue);
        pos.x+=2;
        buff->writeStr(pos, value, termable::color::basic::BoldWhite, termable::color::basic::Blue);
    }
    else {
        buff->writeStr(pos, "[ ] " + value, termable::color::basic::White);
    }
}

int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.showCursor(false);
 
    termable::termBuffer buffer1({40, 3}), buffer2({40, 3});
    buffer1.fill({' '}, termable::color::basic::White, termable::color::basic::Black);
    buffer2.fill({' '}, termable::color::basic::White, termable::color::basic::Black);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
    int selection = 0;
    termable::KeyResult c = std::monostate{};
    
    currBuffer->fill({' '});
    writeStr(currBuffer, {0, 0}, "Option 1 ", selection == 0);
    writeStr(currBuffer, {0, 1}, "Option 2 ", selection == 1);
    term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Rerender);
    // Swap the buffers.
    std::swap(currBuffer, oldBuffer);

    fflush(stdout);

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
                    selection = std::max(0, selection -1);
                    break;
                case termable::NonAsciiChar::Down:
                    selection = std::min(selection +1, 1);
                    break;
            }

            currBuffer->fill({' '});
            writeStr(currBuffer, {0, 0}, "Option 1 ", selection == 0);
            writeStr(currBuffer, {0, 1}, "Option 2 ", selection == 1);
            term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Relative);
            // Swap the buffers.
            std::swap(currBuffer, oldBuffer);
        
            fflush(stdout);
        }

    }
    term.showCursor(true);
    return 0;
}
