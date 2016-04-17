#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <string>
#include <fstream>
#include <sstream>

#include <bcm2835.h>

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