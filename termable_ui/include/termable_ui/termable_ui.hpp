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


struct BorderStyle
{
    termChar topLeft, top, topRight;
    termChar left, right;
    termChar bottomLeft, bottom, bottomRight;
};

void drawBorder(termBuffer& buff, rect r, BorderStyle style);

template<typename T>
class TableView
{
protected:

    virtual std::tuple<termColor, termColor> headerColor(std::size_t colNo) { return {color::basic::Reset, color::basic::Reset}; }
    virtual std::string headerValue(std::size_t colNo) {return ""; }

    virtual std::tuple<termColor, termColor> cellColor(std::size_t rowNo, std::size_t colNo) { return {color::basic::Reset, color::basic::Reset}; }
    virtual std::string cellValue(T& item, std::size_t colNo) { return ""; }

    virtual std::size_t numColumns() const { return 0; }
    virtual std::size_t colSize(std::size_t colNo) const { return 0; }

    void drawRow(termBuffer& buff, T& item, std::size_t rowNo, vec2i drawOffset = {0, 0})
    {
        int xPos = 0;
        for(std::size_t ii = 0; ii < numColumns(); ++ii) {
            const auto [fg, bg] = cellColor(rowNo, ii);
            std::size_t colSz = colSize(ii);
            int numWritten = buff.writeCheckedStr(
                    {xPos+drawOffset.x, rowNo+drawOffset.y}, 
                    cellValue(item, ii),
                    colSz, 
                    fg, bg);
            // Fill the rest of the column
            if(numWritten < colSz) {
                buff.writeCheckedStr(
                    {xPos+numWritten+drawOffset.x, rowNo+drawOffset.y}, 
                    std::string(colSz - numWritten, ' '),
                    fg, bg);
            }
            xPos += colSz;
        }
    }

public:
    TableView() = default;
    virtual ~TableView() = default;

    void drawTable(termBuffer& buff, std::vector<T> items)
    {
        int xPos = 0;
        for(std::size_t ii = 0; ii < numColumns(); ++ii) {
            const auto [fg, bg] = headerColor(ii);
            std::size_t colSz = colSize(ii);
            int numWritten = buff.writeCheckedStr(
                    {xPos, 0}, 
                    headerValue(ii),
                    colSz, 
                    fg, bg);
            // Fill the rest of the column
            if(numWritten < colSz) {
                buff.writeCheckedStr(
                    {xPos+numWritten, 0}, 
                    std::string(colSz - numWritten, ' '),
                    fg, bg);
            }
            xPos += colSz;
        }

        std::size_t rowNo = 0;
        for(auto& item : items) {
            drawRow(buff, item, rowNo, {0, 1});
            rowNo++;
        }
    }

};

// class TableView
// {
// protected:

//     virtual std::tuple<termColor, termColor> headerColor(std::size_t colNo) { return {color::basic::Reset, color::basic::Reset}; }
//     virtual std::string headerValue(std::size_t colNo) = 0;

//     virtual std::tuple<termColor, termColor> cellColor(std::size_t rowNo, std::size_t colNo) { return {color::basic::Reset, color::basic::Reset}; }
//     virtual std::string cellValue(std::size_t rowNo, std::size_t colNo) = 0;

//     virtual std::size_t numRows() const = 0;
//     virtual std::size_t numColumns() const = 0;
//     virtual std::size_t colSize(std::size_t colNo) const = 0;

//     void drawRow(termBuffer& buff, std::size_t rowNo)
//     {
//         int xPos = 0;
//         for(std::size_t ii = 0; ii < numColumns(); ++ii) {
//             const auto [fg, bg] = cellColor(rowNo, ii);
//             std::size_t colSz = colSize(ii);
//             int numWritten = buff.writeCheckedStr(
//                     {xPos, rowNo+1}, 
//                     cellValue(rowNo, ii),
//                     colSz, 
//                     fg, bg);
//             // Fill the rest of the column
//             if(numWritten < colSz) {
//                 buff.writeCheckedStr(
//                     {xPos+numWritten, rowNo+1}, 
//                     std::string(colSz - numWritten, ' '),
//                     fg, bg);
//             }
//             xPos += colSz;
//         }
//     }

// public:
//     TableView() = default;
//     virtual ~TableView() = default;

//     void drawTable(termBuffer& buff)
//     {
//         int xPos = 0;
//         for(std::size_t ii = 0; ii < numColumns(); ++ii) {
//             const auto [fg, bg] = headerColor(ii);
//             std::size_t colSz = colSize(ii);
//             int numWritten = buff.writeCheckedStr(
//                     {xPos, 0}, 
//                     headerValue(ii),
//                     colSz, 
//                     fg, bg);
//             // Fill the rest of the column
//             if(numWritten < colSz) {
//                 buff.writeCheckedStr(
//                     {xPos+numWritten, 0}, 
//                     std::string(colSz - numWritten, ' '),
//                     fg, bg);
//             }
//             xPos += colSz;
//         }

//         for(std::size_t rowNo = 0; rowNo < numRows(); rowNo++) 
//         {
//             drawRow(buff, rowNo);
//         }
//     }

// };

}

}
