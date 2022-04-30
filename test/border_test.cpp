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
    
    termable::ui::drawBorder(*currBuffer, {2, 2, 20, 10}, style);
    
    termable::ui::BorderStyle style2;
    style2.topLeft   = u8"\u250F";
    style2.top       = u8"\u2501";
    style2.topRight  = u8"\u2513";
    style2.left = u8"\u2503";
    style2.right = u8"\u2503";
    style2.bottomLeft   = u8"\u2517";
    style2.bottom       = u8"\u2501";
    style2.bottomRight  = u8"\u251b";
    
    termable::ui::drawBorder(*currBuffer, {24, 2, 30, 15}, style2);

    term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Rerender);
    
    // term.doneRendering(*currBuffer, termable::BufferRenderOption::Origin);
    term.showCursor(true);
    return 0;
}
