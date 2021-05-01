// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>

void wipeScreen(termable::termable& term, uint8_t color, termable::vec2i size) {
    for(int ii = 0; ii < size.y-1; ii++) {
        // Print a line on the screen
        term.setBackgroundColor(color);
        //printf("\r%s",termable::color::background::color256(color).c_str());
        for(int jj = 0; jj < size.x-1; jj++) {
            printf(" ");
        }
        //printf("%s\n", termable::color::ResetColor);
        term.setBackgroundColor(termable::color::basic::Reset);
        printf("\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char** argv)
{
    termable::termableLinux term;
    
    auto sz = term.displaySize();
    for(uint8_t ii = 100; ii < 116; ii++) {
        term.setCursorPos({0,0});
        wipeScreen(term, ii, sz);
    }

    term.clear();
    printf("Terminal size = %d x %d\n", sz.x, sz.y);

    
    for(int ii = 0; ii < 16; ii++) {
        for(int jj = 0; jj < 16; jj++) {
            term.setForegroundColor(static_cast<unsigned char>(ii*16+jj));
            printf("%s", u8"\u2589");
        }
        term.setBackgroundColor(termable::color::basic::Reset);
        printf("\n");
    }
    printf("\n");
    for(int ii = 0; ii < 16; ii++) {
        for(int jj = 0; jj < 16; jj++) {
            term.setBackgroundColor(static_cast<unsigned char>(ii*16+jj));
            printf(" %03d ", ii*16+jj);
        }
        term.setBackgroundColor(termable::color::basic::Reset);
        printf("\n");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    term.moveUp(3);
    printf("Moved up!");
    fflush(stdout);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    term.moveDown(1);
    printf("Moved down!");
    fflush(stdout);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    term.moveLeft(10);
    printf("Moved Left!");
    fflush(stdout);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    term.moveRight(15);
    printf("Moved Right!");
    fflush(stdout);

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    term.setCursorPos({0,0});
    

    auto termBuf = termable::termBuffer(sz);
    termBuf.fill(' ');
    termBuf.horzLine(termable::utf8Char(u8"\u2589"), {0,0}, sz.x);
    termBuf.horzLine(termable::utf8Char(u8"\u2589"), {0,sz.y-1}, sz.x);
    termBuf.vertLine(termable::utf8Char(u8"\u2589"), {0,1}, sz.y-2);
    termBuf.vertLine(termable::utf8Char(u8"\u2589"), {sz.x-1,1}, sz.y-2);
    term.renderBuffer(termBuf, termable::BufferRenderOption::Origin);

    termBuf.filledRect(termable::utf8Char('@'), {2, 2}, {30, 50});
    termBuf.filledRect(termable::utf8Char('.'), {6, 7}, {40, 20});
    
    term.renderBuffer(termBuf, termable::BufferRenderOption::Origin);
    // for(int jj = 0; jj < sz.x; jj++) printf(u8"\u2589");
    // printf("\n");
    // for(int yy = 0; yy < sz.y-2; yy++) {
    //     printf("%s%s%s\n", u8"\u2589", std::string(sz.x-2, ' ').c_str(), u8"\u2589");
    // }
    // for(int jj = 0; jj < sz.x; jj++) printf(u8"\u2589");
    // printf("\n");
    term.showCursor(true);
}
