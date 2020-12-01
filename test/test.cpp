// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>

void wipeScreen(uint8_t color, termable::vec2i size) {
    for(int ii = 0; ii < size.y-1; ii++) {
        // Print a line on the screen
        printf("\r%s",termable::color::foreground::color256(color).c_str());
        for(int jj = 0; jj < size.x-1; jj++) {
            printf(u8"\u2589");
        }
        printf("%s\n", termable::color::ResetColor);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char** argv)
{
    termable::termableLinux term;
    
    auto sz = term.displaySize();
    for(uint8_t ii = 100; ii < 116; ii++) {
        term.setCursorPos({0,0});
        wipeScreen(ii, sz);
    }

    term.clear();
    printf("Terminal size = %d x %d\n", sz.x, sz.y);

    for(int ii = 0; ii < 16; ii++) {
        for(int jj = 0; jj < 16; jj++) {
            printf("%s%s",  termable::color::foreground::color256(ii*16+jj).c_str(), u8"\u2589");
        }
        printf("%s\n", termable::color::ResetColor);
    }
    printf("\n");
    for(int ii = 0; ii < 16; ii++) {
        for(int jj = 0; jj < 16; jj++) {
            printf("%s %03d ",  termable::color::background::color256(ii*16+jj).c_str(), ii*16+jj);
        }
        printf("%s\n", termable::color::ResetColor);
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
    for(int jj = 0; jj < sz.x; jj++) printf(u8"\u2589");
    printf("\n");
    for(int yy = 0; yy < sz.y-2; yy++) {
        printf("%s%s%s\n", u8"\u2589", std::string(sz.x-2, ' ').c_str(), u8"\u2589");
    }
    for(int jj = 0; jj < sz.x; jj++) printf(u8"\u2589");
    printf("\n");
}
