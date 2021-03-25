#include <pigpio.h>
#include "CControlPi.h"
#include <opencv2/opencv.hpp>
#include <iostream>

CControlPi::CControlPi(){
    gpioSetMode(S1, PI_INPUT);
    gpioSetMode(S2, PI_INPUT);
    gpioSetMode(SERVO2, PI_OUTPUT);
    gpioSetMode(LED1, PI_OUTPUT);
}

CControlPi::~CControlPi(){
    gpioTerminate();
}

bool CControlPi::get_data(int type, int channel, int& result)
{
    if(type == DIGITAL)
    {
        result = gpioRead(channel);
    }
    else if(type == SERVO)
    {
        result = gpioGetServoPulsewidth(channel);
    }
    else if(type == ANALOG)
    {
        int read_val;
        unsigned char inBuf[3];

        char cmd[3];
        cmd[0] = 1;
        cmd[1] = (0b1000 | channel) << 4;
        cmd[3] = 0;

        //char cmd[] = { 1, 128, 1 }; // 0b1XXX0000 where XXX=channel 0
        int handle = spiOpen(0, 200000, 3); //   Mode 0, 200kHz
        spiXfer(handle, cmd, (char*) inBuf, 3); //   Transfer 3 bytes
        read_val = ((inBuf[1] & 3) << 8) | inBuf[2]; //   Format 10 bits
        spiClose(handle); //   Close SPI system

        result = read_val;
    }

    if (result == PI_BAD_GPIO)
    {
        result = -1;
        return false;
    }
    return true;
}

bool CControlPi::set_data(int type, int channel, int val)
{
    if(type == SERVO)
    {
        gpioServo(channel, val);
        return true;
    }
    else if(type == DIGITAL)
    {
        if(gpioWrite(channel, val) == 0)
        {
            return true;
        }
    }
    return false;
}

bool CControlPi::get_analog(int channel, int& result)
{
    if (get_data(ANALOG, channel, result))
    {
        result = (result*100)/1024;
        return true;
    }
    return false;
}

bool CControlPi::get_button(int channel)
{
    int result;
    get_data(DIGITAL, channel, result);

    if (result == 1)
    {
        return false;
    }
    return true;
}
