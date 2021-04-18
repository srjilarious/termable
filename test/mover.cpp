// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>


int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();
    char c = 0;
    while(c != -1) {
        c = term.getch();
        //if(c == 'q') {
            printf("%d, ", c);
            fflush(stdout);
        //}
    }
    return 0;
}
