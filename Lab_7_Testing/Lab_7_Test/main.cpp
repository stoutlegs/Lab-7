#include <pigpio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "CControlPi.h"

using namespace std;

void print_menu()
{
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout << "\n~ELEX 4618 Lab 3 - Michael Andrews";
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	cout << "\n\nPress A for joystick sampling";
	cout << "\nPress B for digital button state sampling";
	cout << "\nPress C button press counting";
	cout << "\nPress D to rotate the servo\n\n";
}

int main(int argc, char* argv[])
{
    if(gpioInitialise() < 0)
    {
        return 1;
    }

    gpioSetMode(18, PI_OUTPUT);
    gpioSetMode(23, PI_OUTPUT);
    gpioSetMode(24, PI_OUTPUT);

	char cmd;
    CControlPi myController;

    std::string window = "window";

    cv::namedWindow(window, CV_WINDOW_NORMAL);

	int type;
	int channel;
	int value;
	int percent;
	int count;
	bool pressed = false;
	char key;


	do
	{
		print_menu();
		std::cin >> cmd;
		switch (cmd)
		{
		case 'A':
		case 'a':
            do
            {
                type = 1;

                channel = 0;
                myController.get_data(ANALOG, channel, value);
                myController.get_analog(channel, percent);
                cout << "Analog Test: " << channel << " = " << value << " (" << percent << "%) ";

                channel = 1;
                myController.get_data(ANALOG, channel, value);
                myController.get_analog(channel, percent);
                cout << "Analog Test: " << channel << " = " << value << " (" << percent << "%) ";
                cout << "\n";

                key = cv::waitKey(50);
            }while(key != 'q');
			break;

		case 'B':
		case 'b':
			type = DIGITAL;

            do
            {
                channel = S1;
                myController.get_data(type, channel, value);
                channel = LED1;
                myController.set_data(type, channel, value);
                cout << "Digital Test: " << channel << " = " << value;
                cout << "\n";
                key = cv::waitKey(50);
            }while(key != 'q');
			channel = LED1;
			value = 0;
			myController.set_data(type, channel, value);
			break;
		case 'C':
		case 'c':
            gpioWrite(23, 1);
            gpioWrite(24, 0);
			gpioPWM(18, 20
			);
			break;
		case 'D':
		case 'd':

			type = SERVO;
			channel = SERVO2;
			bool direction = true;
			myController.set_data(type, channel, SERVO_MIN);
            do
            {
                myController.get_data(type, channel, value);

                if (direction)
                {
                    value += 100;
                }
                else
                {
                    value -= 100;
                }

                myController.set_data(type, channel, value);

                cout << "SERVO TEST: " << channel << " POS " << value << "\n";

                myController.get_data(type, channel, value);

                if (value >= SERVO_MAX)
                {
                    direction = false;
                }
                else if (value <= SERVO_MIN)
                {
                    direction = true;
                }

                key = cv::waitKey(500);

            }while(key != 'q');
			break;
		}
	} while (cmd != 'Q');
}
