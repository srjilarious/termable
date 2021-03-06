#pragma once

#include <vector>
#include <string>

#include <termable/termable.hpp>

namespace termable
{

namespace ui
{

struct MenuModel
{
    std::vector<std::string> options;
    int selection = -1;
    vec2i offset;
};

class InlineMenuView 
{
private:
    MenuModel& mModel;

    void writeStr(
        termBuffer& buff, 
        vec2i pos, 
        std::string value, 
        bool selected);
        
public:
    InlineMenuView(MenuModel& model);
    bool handleInput(KeyResult c);
    void render(termBuffer& buffer);
};

class FullMenuView 
{
private:
    MenuModel& mModel;
    vec2i mSize;

    void writeStr(
        termBuffer& buff, 
        vec2i pos, 
        std::string value, 
        bool selected);
        
public:
    FullMenuView(MenuModel& model, vec2i size);
    bool handleInput(KeyResult c);
    void render(termBuffer& buffer);
};

}

}
