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
 
    termable::ui::MenuModel model;
    model.options.push_back("Option 1");
    model.options.push_back("Option 2");
    model.options.push_back("Option 3");
    model.options.push_back("Option 4");
    model.options.push_back("Option 5");
    model.selection = 0;

    auto sz = term.displaySize();

    termable::termBuffer buffer1(sz), buffer2(sz);
    buffer1.fill({' '}, termable::color::basic::White, termable::color::basic::Black);
    buffer2.fill({'.'}, termable::color::basic::White, termable::color::basic::Black);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
    

    termable::ui::FullMenuView menu(model, sz);
    menu.render(*currBuffer);

    term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Rerender);
    // Swap the buffers.
    std::swap(currBuffer, oldBuffer);

    fflush(stdout);

    termable::KeyResult c = term.getch();
    while(!menu.handleInput(c)) {
        if(std::holds_alternative<std::monostate>(c))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else {
            menu.render(*currBuffer);
            term.renderBuffer(*currBuffer, *oldBuffer/*, termable::BufferRenderOption::Relative*/);
            // Swap the buffers.
            std::swap(currBuffer, oldBuffer);
        
            fflush(stdout);
        }

        c = term.getch();
    }
    
    term.doneRendering(*currBuffer, termable::BufferRenderOption::Relative);
    term.showCursor(true);
    return 0;
}
