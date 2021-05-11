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

FullMenuView::FullMenuView(MenuModel& model, vec2i size)
    : mModel(model),
      mSize(size)
{
}

void 
FullMenuView::writeStr(
        termBuffer &buff, 
        vec2i pos, 
        std::string value, 
        bool selected)
{
    // 10 padding, 4 for selection portion
    auto amountLeft = mSize.x-10-4; 
    auto amountStr = std::string(amountLeft-value.size(), ' ');
    if(selected)
    {
        buff.writeCheckedStr(pos, "[", color::basic::BoldYellow, color::basic::Red);
        pos.x++;
        buff.writeCheckedStr(pos, "*", color::basic::BoldCyan, color::basic::Red);
        pos.x++;
        buff.writeCheckedStr(pos, "] ", color::basic::BoldYellow, color::basic::Red);
        pos.x+=2;
        buff.writeCheckedStr(pos, value + amountStr, color::basic::BoldWhite, color::basic::Red);
    }
    else {
        buff.writeCheckedStr(pos, "[ ] " + value + amountStr, color::basic::Black, color::basic::White);
    }
}

bool
FullMenuView::handleInput(KeyResult c)
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
FullMenuView::render(termBuffer& buffer)
{
    buffer.fill({' '}, color::basic::White, color::basic::Blue);
    // TODO: calc how much padding we can afford.
    vec2i padding = {3, 3};
    buffer.filledRect(' ', 
            {padding.x, padding.y}, {mSize.x-padding.x, mSize.y-padding.y},
            color::basic::White, 
            color::basic::White);
    vec2i textStart = {padding.x+2, padding.y+2};
    for(size_t ii = 0; ii < mModel.options.size(); ii++) {
        writeStr(
                buffer, 
                {textStart.x, textStart.y + static_cast<int>(ii)}, 
                mModel.options[ii], 
                mModel.selection == ii
            );
    }
}

}
}
