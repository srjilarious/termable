// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <termable_ui/termable_ui.hpp>
#include <cstdio>
#include <chrono>
#include <thread>
#include <algorithm>

int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.showCursor(false);
 
    auto sz = term.displaySize();

    termable::termBuffer buffer1(sz), buffer2(sz);
    buffer1.fill({' '}, termable::color::basic::White, termable::color::basic::Black);
    buffer2.fill({'.'}, termable::color::basic::White, termable::color::basic::Black);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
    
    termable::ui::BorderStyle style;
    style.topLeft   = '/';
    style.top       = '-';
    style.topRight  = '\\';
    style.left = '|';
    style.right = '|';
    style.bottomLeft   = '\\';
    style.bottom       = '=';
    style.bottomRight  = '/';
    
    termable::ui::drawBorder(*currBuffer, {5, 2, 30, 10}, style);

    term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Rerender);
    
    // term.doneRendering(*currBuffer, termable::BufferRenderOption::Origin);
    term.showCursor(true);
    return 0;
}
