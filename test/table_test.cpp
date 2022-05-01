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

class MyTable : public termable::ui::TableView
{
public:
    MyTable() = default;
    virtual ~MyTable() = default;

    std::vector<Item> items;

protected:
    std::tuple<termable::termColor, termable::termColor> headerColor(std::size_t colNo) override;
    std::string headerValue(std::size_t colNo) override;

    std::tuple<termable::termColor, termable::termColor> cellColor(std::size_t rowNo, std::size_t colNo) override;
    std::string cellValue(std::size_t rowNo, std::size_t colNo) override;

    virtual std::size_t numRows() const override { return items.size(); }
    std::size_t numColumns() const override { return 3; }
    std::size_t colSize(std::size_t colNo) const override;
};

std::tuple<termable::termColor, termable::termColor> 
MyTable::headerColor(std::size_t colNo)
{
    return {termable::color::basic::Blue, termable::color::basic::BoldYellow};
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
    if((rowNo % 2) == 0) {
        return {termable::color::basic::BoldWhite, termable::color::basic::Blue};
    }
    else {
        return {termable::color::basic::BoldWhite, termable::color::basic::Green};
    }
}

std::string 
MyTable::cellValue(std::size_t rowNo, std::size_t colNo)
{
    auto& item = items[rowNo];
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
    buffer1.fill({' '}, termable::color::basic::White, termable::color::basic::Black);
    buffer2.fill({'.'}, termable::color::basic::White, termable::color::basic::Black);

    termable::termBuffer *currBuffer = &buffer1;
    termable::termBuffer *oldBuffer = &buffer2;
        
    MyTable table;
    table.items.push_back({0, "downloads/", "DIR"});
    table.items.push_back({1, "audio/", "DIR"});
    table.items.push_back({2, "my_doc.txt", "Text"});
    table.items.push_back({3, "super_script.sh", "Script"});

    table.drawTable(*currBuffer);

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
