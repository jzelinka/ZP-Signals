#include <navestidla_arduino.h>

// TODO just setup the address here
#define MY_ADDRESS 0x02

void setup()
{
    navestidla_setup(MY_ADDRESS);
}

void loop()
{
    navestidla_loop();
}