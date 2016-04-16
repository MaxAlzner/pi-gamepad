
#include "../include/pipad.h"

namespace pipad
{
    void start()
    {
        bcm2835_init();
    }
    void close()
    {
        bcm2835_close();
    }
    
    void poll(const gamepad_t& e)
    {
        return;
    }
}