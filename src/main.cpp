
#include "../include/pipad.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    pipad::start();
    while (getchar() != 27)
    {
        static pipad::gamepad_t pad;
        pipad::poll(pad);
        printf("  gamepad : \n");
        printf("    a          = %d\n", pad.a);
        printf("    b          = %d\n", pad.b);
        printf("    x          = %d\n", pad.x);
        printf("    y          = %d\n", pad.y);
        printf("    start      = %d\n", pad.start);
        printf("    back       = %d\n", pad.back);
        printf("    shoulder L = %d\n", pad.shoulderL);
        printf("    shoulder R = %d\n", pad.shoulderR);
        printf("    thumb L X  = %d\n", pad.thumbLX);
        printf("    thumb L Y  = %d\n", pad.thumbLY);
        printf("    thumb R X  = %d\n", pad.thumbRX);
        printf("    thumb R Y  = %d\n", pad.thumbRY);
        printf("    trigger L  = %d\n", pad.triggerL);
        printf("    trigger R  = %d\n", pad.triggerR);
        printf("    value      = %d\n", pad.volume);
        printf("    battery    = %d\n", pad.battery);
    }
    
    pipad::close();
    return 0;
}