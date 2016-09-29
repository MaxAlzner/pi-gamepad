#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <string>
#include <fstream>
#include <sstream>

#include <wiringPi.h>

namespace pipad
{
    typedef enum
    {
        GPIO_GAMEPAD_INVALID = -1,
        GPIO_GAMEPAD_MCP3008_1_CLK = 0,
        GPIO_GAMEPAD_MCP3008_1_DOUT = 1,
        GPIO_GAMEPAD_MCP3008_1_DIN = 2,
        GPIO_GAMEPAD_MCP3008_1_CS = 3,
        GPIO_GAMEPAD_CD4021_1_PSC = 4,
        GPIO_GAMEPAD_CD4021_1_CLK = 5,
        GPIO_GAMEPAD_CD4021_1_DATA = 6
    } GPIO_GAMEPAD;
    
    struct gamepad_t
    {
        union
        {
            struct
            {
                uint8_t a, b, x, y;
                uint8_t shoulderL, shoulderR;
                uint8_t start, back;
            };
            uint64_t buttonpad;
            uint8_t buttons[8];
        };
        union
        {
            struct
            {
                uint16_t thumbLX, thumbLY;
                uint16_t thumbRX, thumbRY;
                uint16_t triggerL, triggerR;
                uint16_t volume, battery;
            };
            uint16_t analogs[8];
        };
    };
    
    extern void start();
    extern void close();
    
    extern void poll(gamepad_t& e);
}