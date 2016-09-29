
#include "../include/pipad.h"
#include <stdio.h>
#include <time.h>
#include <curses.h>

int main(int argc, char** argv)
{
    pipad::start();
    WINDOW* win = initscr();
    noecho();
    wclear(win);
    time_t start;
    time(&start);
    double last = 0.0;
    while ((last = difftime(time(0), start)) <= 10.0)
    {
        static pipad::gamepad_t pad;
        pipad::poll(pad);
        wmove(win, 0, 0);
        printw("%f second(s)\n", last);
        printw("\n");
#if 1
        printw("  gamepad : \n");
        printw("    a          = %d\n", pad.a);
        printw("    b          = %d\n", pad.b);
        printw("    x          = %d\n", pad.x);
        printw("    y          = %d\n", pad.y);
        printw("    start      = %d\n", pad.start);
        printw("    back       = %d\n", pad.back);
        printw("    shoulder L = %d\n", pad.shoulderL);
        printw("    shoulder R = %d\n", pad.shoulderR);
        printw("    thumb L X  = %d\n", pad.thumbLX);
        printw("    thumb L Y  = %d\n", pad.thumbLY);
        printw("    thumb R X  = %d\n", pad.thumbRX);
        printw("    thumb R Y  = %d\n", pad.thumbRY);
        printw("    trigger L  = %d\n", pad.triggerL);
        printw("    trigger R  = %d\n", pad.triggerR);
        printw("    volume     = %d\n", pad.volume);
        printw("    battery    = %d\n", pad.battery);
#endif
        wrefresh(win);
    }
    
    endwin();
    pipad::close();
    return 0;
}