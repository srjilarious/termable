// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <termable_ui/termable_ui.hpp>
#include <cstdio>
#include <chrono>
#include <thread>
#include <algorithm>

struct Item
{
    int id;
    std::string name, type;
};

class MyTable : public termable::ui::TableView<Item>
{
public:
    MyTable() = default;
    virtual ~MyTable() = default;

protected:
    std::tuple<termable::termColor, termable::termColor> headerColor(std::size_t colNo) override;
    std::string headerValue(std::size_t colNo) override;

    std::tuple<termable::termColor, termable::termColor> cellColor(std::size_t rowNo, std::size_t colNo) override;
    std::string cellValue(Item& item, std::size_t colNo) override;

    std::size_t numColumns() const override { return 3; }
    std::size_t colSize(std::size_t colNo) const override;
};

std::tuple<termable::termColor, termable::termColor> 
MyTable::headerColor(std::size_t colNo)
{
    if(colNo == 1) {
        return {
            termable::termColor(termable::color::basic::BoldYellow, termable::TermStyle::Underline), 
            termable::termColor((uint8_t)26)
        };
    }
    else {
        return {
            termable::termColor(termable::color::basic::BoldYellow, termable::TermStyle::Bright), 
            termable::termColor((uint8_t)26)
        };
    }
}

std::string 
MyTable::headerValue(std::size_t colNo)
{
    switch(colNo) {
        case 0:
            return "ID";
        
        case 1:
            return "Name";
        
        case 2:
            return "Type";

        default: 
            return "";
    }
}

std::tuple<termable::termColor, termable::termColor> 
MyTable::cellColor(std::size_t rowNo, std::size_t colNo) 
{
    // Highlight the second column
    if(colNo == 1) {
        if((rowNo % 2) == 0) {
            auto bg = termable::termColor((uint8_t)17);
            return {termable::color::basic::BoldWhite, bg};
        }
        else {
            auto bg = termable::termColor{(uint8_t)18};
            return {termable::color::basic::BoldWhite, bg};
        }
    }
    else {
        if((rowNo % 2) == 0) {
            auto bg = termable::termColor((uint8_t)235);
            return {termable::color::basic::BoldWhite, bg};
        }
        else {
            auto bg = termable::termColor{(uint8_t)238};
            return {termable::color::basic::BoldWhite, bg};
        }
    }
}

std::string 
MyTable::cellValue(Item& item, std::size_t colNo)
{
    switch(colNo) {
        case 0:
            return std::to_string(item.id);
        
        case 1:
            return item.name;
        
        case 2:
            return item.type;

        default: 
            return "";
    }
}


std::size_t 
MyTable::colSize(std::size_t colNo) const
{
    switch(colNo) {
        case 0:
            return 4;
        
        case 1:
            return 30;
        
        case 2:
            return 15;

        default: 
            return 0;
    }
}

int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.showCursor(false);
 
    auto sz = term.displaySize();

    termable::termBuffer buffer1(sz), buffer2(sz);
    buffer1.fill({' '}, termable::color::basic::White, termable::color::basic::Reset);
    buffer2.fill({'.'}, termable::color::basic::White, termable::color::basic::Reset);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
        
    std::vector<Item> items;
    items.push_back({0, "downloads/", "DIR"});
    items.push_back({1, "audio/", "DIR"});
    items.push_back({2, "my_doc.txt", "Text"});
    items.push_back({3, "super_script.sh", "Script"});

    MyTable table;
    table.drawTable(*currBuffer, items);

    term.renderBuffer(*currBuffer, *oldBuffer, termable::BufferRenderOption::Rerender);
    
    while(true) {
        termable::KeyResult c = term.getch();
        if(std::holds_alternative<std::monostate>(c))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else if(std::holds_alternative<char>(c)) {
            auto ch = std::get<char>(c);
            if(ch == 'q' || ch == '\033') {
                break;
            }
        }
    }

    // term.doneRendering(*currBuffer, termable::BufferRenderOption::Origin);
    term.showCursor(true);
    return 0;
}
