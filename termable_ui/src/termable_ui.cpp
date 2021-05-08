#include <termable/termable.hpp>
#include <termable_ui/termable_ui.hpp>
#include <algorithm>

namespace termable
{
namespace ui
{

InlineMenuView::InlineMenuView(MenuModel& model)
    : mModel(model)
{
}

void 
InlineMenuView::writeStr(
        termBuffer &buff, 
        vec2i pos, 
        std::string value, 
        bool selected)
{
    if(selected)
    {
        buff.writeStr(pos, "[", color::basic::BoldYellow, color::basic::Blue);
        pos.x++;
        buff.writeStr(pos, "*", color::basic::BoldCyan, color::basic::Blue);
        pos.x++;
        buff.writeStr(pos, "] ", color::basic::BoldYellow, color::basic::Blue);
        pos.x+=2;
        buff.writeStr(pos, value, color::basic::BoldWhite, color::basic::Blue);
    }
    else {
        buff.writeStr(pos, "[ ] " + value, color::basic::White);
    }
}

bool
InlineMenuView::handleInput(KeyResult c)
{
    if(std::holds_alternative<char>(c)) {
        auto ch = std::get<char>(c);
        if(ch == 'q' || ch == '\033') {
            return true;
        }
        else if(ch == '\n') {
            return true;
        }
    }
    else if(std::holds_alternative<NonAsciiChar>(c)) {
        auto nac = std::get<NonAsciiChar>(c);

        switch(nac) {
            case NonAsciiChar::Up:
                mModel.selection = std::max(0, mModel.selection -1);
                break;
            case NonAsciiChar::Down:
                mModel.selection = std::min(mModel.selection +1, 
                        static_cast<int>(mModel.options.size()-1));
                break;
        }
    }
    return false;
}

void
InlineMenuView::render(termBuffer& buffer)
{
    buffer.fill({' '});
    for(size_t ii = 0; ii < mModel.options.size(); ii++) {
        writeStr(
                buffer, 
                {0, static_cast<int>(ii)}, 
                mModel.options[ii], 
                mModel.selection == ii
            );

    }
}

}
}
