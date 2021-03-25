#pragma once

#define DIGITAL 0
#define ANALOG 1
#define SERVO 2

#define ON 1
#define OFF 0

#define LED1 16

#define JS_HORI 0
#define JS_VERT 1

#define S1 23
#define S2 21

#define SERVO2 13
#define SERVO_MAX 1000
#define SERVO_MIN 2000

/**
*
* @brief Used to communicate with the embedded system
*
* This class is meant to contain all information and
* functions related to serial communication with the
* embedded system.
*
* @author Michael Andrews
*
*/
class CControlPi
{
private:
    int handle;
public:

    CControlPi();

    ~CControlPi();

    /** @brief This method will get data from the serial port
    *
    * @param type This is the type of data being sent
    * @param channel This is the channel of the microcontroller to be used
    * @param result This will store the result
    * @return Returns if it was successful or not
    */
    bool get_data(int type, int channel, int& result);

    /** @brief This method will set the data from the serial port
    *
    * @param type This is the type of data being sent
    * @param channel This is the channel of the microcontroller to be used
    * @param result This will store the result
    * @return Returns if it was successful or not
    */
    bool set_data(int type, int channel, int val);

    /** @brief This method will call the get data function for analog only then convert it to a percent
    *
    * @param channel This is the channel of the microcontroller to be used
    * @param result This will store the result
    * @return Returns if it was successful or not
    */
    bool get_analog(int channel, int& result);

    bool get_button(int channel);
};

