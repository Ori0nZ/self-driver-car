#include <string>
#include <sstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "test_cam.h"
#include <wiringSerial.h>

using namespace std;
using namespace cv;
using namespace raspicam;

Test_Cam *test_cam = new Test_Cam;
int port;

bool setup()
{
	cout << "Arduino connection ... ";
	if ((port = serialOpen ("/dev/ttyS0", 9600)) < 0)   // Open serial0 port with 9600 baudrate.
	{
		cout << "Unable to connect\n";
		return false;
	}
	cout << "Successful\n" << endl;
	return true;
}


int main()
{
	string command;
	if (!setup())
	{
		cout << "ERROR\n";
		return 0;
	}
	//cout << "Enter command: ";
	//cin >> command;
	test_cam->left(port);
	return 0;
}

