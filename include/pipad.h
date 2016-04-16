#pragma once

#include <stdint.h>
#include <unistd.h>

#include <bcm2835.h>

namespace pipad
{
    struct gamepad_t
    {
        uint8_t a, b, x, y;
        uint8_t shoulderL, shoulderR;
        uint8_t start, back;
        uint16_t thumbLX, thumbLY;
        uint16_t thumbRX, thumbRY;
        uint16_t triggerL, triggerR;
        uint16_t volume, battery;
    };
    
    extern void start();
    extern void close();
    
    extern void poll(const gamepad_t& e);
}