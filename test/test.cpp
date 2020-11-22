// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    termable::termableLinux term;
    auto sz = term.displaySize();
    printf("Terminal size = %d x %d\n", sz.x, sz.y);

    for(int ii = 0; ii < 16; ii++) {
        for(int jj = 0; jj < 16; jj++) {
            printf("%s%s",  termable::color::color256(ii*16+jj).c_str(), u8"\u2589");
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
}
