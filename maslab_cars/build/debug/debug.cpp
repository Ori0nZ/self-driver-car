#include <stdlib.h> // rand() function
#include <unistd.h> // usleep() function
#include <string> // string type
//#include "car.h" // Logic class
#include "../headers/test_cam.h" // Camera class
//#include "../headers/line_reco.h"
#include <stdio.h> // printf(), fflush() functions
#include <wiringSerial.h> // serialOpen(), serialPutchar() functions
//#include <fstream> // ofstream type, .open() function
#include <iostream> // cout function
#include <ctime>

using namespace std;
using namespace raspicam;

//Logic *logic = new Logic; // Includes class from car.h
Test_Cam *test_cam = new Test_Cam;


int serial_port; // Serial port number
int usb_port; // Usb port number


bool setup()
{
        cout << "Arduino connection ... ";
        if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)   // Open serial0 port with 9600 baudrate.
        {
                cout << "Unable to connect\n";
                return false;
        }
        cout << "Successful" << endl;

        cout << "Xbee connection ... ";
        if ((usb_port = serialOpen ("/dev/ttyUSB0", 9600)) < 0)   // Open ttyUSB0 port with 9600 baudrate.
        {
                if ((usb_port = serialOpen ("/dev/ttyUSB1", 9600)) < 0)   // Open ttyUSB1 port with 9600 baudrate.
                {
                        cout << "Unable to connect\n";
                        return false;
                }
        }
        cout << "Successful" << endl;
        return true;
}

void cmd(string str)
{
        while (str.length() > 0)
        {
                string com;
                int i = 0;
                do
                {
                        com[i] = str[i];
                        i++;
                }
                while (str[i] != '.');
                str.erase(0, i + 1);
		if (com[0] == 'l')
                {
                        test_cam->left(serial_port);
                }
                else if (com[0] == 'r')
                {
                        test_cam->right(serial_port);
                }
                else if (com[0] == 'f')
                {
                        test_cam->forward(serial_port, com[4] - '0');
                }
        }
}


int main()
{
        //fout.open("logs.txt");
        cout << "Setup ... " << endl;
        if (!setup())
        {
                cout << "ERROR\n";
                return 0;
        }
       // logic->start();
        cout << "Done" << endl;
        //cout << "Recieves own id ..." << endl;
        //getid();
        //test_cam->line_detection();
	cout << "Done" << endl;
        while (true)
        {
                //cout << endl << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl << endl;
                //cout << "Path: \n" << logic->getcoord() << endl;
                //cout << "Commands: \n" << logic->getcmds() << endl;
                //cmd("fwd(5).lft.fwd(4).rgt.fwd(4).lft.fwd(5).lft.fwd(5).lft.fwd(9).");
                //cmd("fwd(1).lft.fwd(1).lft.fwd(5).lft.fwd(4).rgt.fwd(4).lft.fwd(1).lft.fwd(4).rgt.fwd(4).lft.fwd(5).lft.fwd(5).lft.fwd(9).");
		time_t t_start = time(0);
		//cmd("fwd(1).lft.fwd(1).lft.fwd(5).lft.ct0.fwd(4).rgt.fwd(4).lft.fwd(1).lft.ct0.fwd(4).rgt.fwd(4).lft.fwd(1).lft.ct0.fwd(4).rgt.fwd(4).lft.fwd(1).lft.ct0.fwd(4).rgt.fwd(4).lft.fwd(5).lft.ct0.fwd(9).lft.ct0.fwd(9).lft.ct0.fwd(8).lft.fwd(1).");
		//cmd("fwd(3).");
		//cmd("fwd(2).");
		//usleep(2000000);
		//cmd("fwd(3).");
		//usleep(2000000);
		cmd("fwd(1).");
		cout << "TIME: " << time(0) - t_start << " seconds\n";
		return 0;
		usleep (2000000);
                //logic->newpath();
        }
        return 0;
}

