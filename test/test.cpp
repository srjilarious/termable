// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <stdio.h>

int main(int argc, char** argv)
{
    termable::termableLinux term;
    auto sz = term.displaySize();
    printf("Terminal size = %d x %d\n", sz.x, sz.y);
}
