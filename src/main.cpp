
#include "../include/pipad.h"

int main(int argc, char** argv)
{
    bcm2835_init();
    bcm2835_close();
    return 0;
}