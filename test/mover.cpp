// Set of tests, blocking out how I want the API to look.

#include <termable/termable.hpp>
#include <cstdio>
#include <chrono>
#include <thread>


int main(int argc, char** argv)
{
    termable::termableLinux term;
    term.clear();
    termable::KeyResult c = std::monostate{};
    while(true) {
        c = term.getch();
        if(std::holds_alternative<char>(c)) {
            auto ch = std::get<char>(c);
            printf("%d, ", ch);
            fflush(stdout);

            if(ch == 'q') {
                break;
            }
        }
        else if(std::holds_alternative<termable::NonAsciiChar>(c)) {
            auto nac = std::get<termable::NonAsciiChar>(c);
            switch(nac) {
                case termable::NonAsciiChar::Up:
                    printf("!Up! ");
                    break;
                case termable::NonAsciiChar::Left:
                    printf("!Left! ");
                    break;
                case termable::NonAsciiChar::Down:
                    printf("!Down! ");
                    break;
                case termable::NonAsciiChar::Right:
                    printf("!Right! ");
                    break;
                
            }
            fflush(stdout);
        }

    }
    return 0;
}
