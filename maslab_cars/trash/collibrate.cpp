#include <iostream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <wiringSerial.h>
#include <raspicam/raspicam.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <cmath>
#include "b_w.h"

using namespace std;
using namespace cv;
using namespace raspicam;

RaspiCam Picamera;

B_W *b_w = new B_W;

/*
* Open camera stream with special settings.
*/
void setup()
{
	Picamera.setFormat(RASPICAM_FORMAT_BGR);
	Picamera.setCaptureSize(800, 400);
	Picamera.setSaturation(0);
	Picamera.setBrightness(70);
	Picamera.setISO(800);
	Picamera.setExposure(RASPICAM_EXPOSURE_OFF);
	Picamera.setRotation(180);

	cout << "Opening video stream\n";
	if (!Picamera.isOpened())
	{
		if ( !Picamera.open())
		{
			cout << "Unable to open\n";
		}
	}
}

/*
* Close camera stream.
*/
void setdestruct()
{
	cout << "Closing video stream\n";
	Picamera.release();
}


/*
* Move forward. It count squares and correct loacation, depends on squares boards.
*/
void line_detection ()
{
	setup();
	float x_left_up, x_left_bottom, x_right_up, x_right_bottom;
	while(1)
	{
		Picamera.grab();
		unsigned char *data=new unsigned char[Picamera.getImageTypeSize(RASPICAM_FORMAT_BGR)];
		Picamera.retrieve (data);
		Mat frame(400, 800, CV_8UC3);
		memcpy(frame.data, data, 400*800*3*sizeof(unsigned char));
		delete data;

		Mat blur, mask;
		Rect roi(0, 150, frame.size().width, 100);
		Mat frame_roi = frame(roi);
		static const int arr[] = {0, 0, 0};
		vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
		static const int arr1[] = {150, 150, 150};
		vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
		bilateralFilter(frame_roi, blur, 15, 75, 75);
		inRange(blur, lower_white, upper_white, mask);

		vector <float> coefficients(4);
		b_w->color_detect(mask, "");
		coefficients = b_w->get_coefficients();
		coefficients[1] += 150;
		coefficients[3] += 150;

		float k_left = coefficients[0];
		float b_left = coefficients[1];
		float k_right = coefficients[2];
		float b_right = coefficients[3];

		if (abs(k_left) < 0.01 || abs(k_right) < 0.01 )
		{
			cout << "LINE WAS NOT FOUND! HELP ME!";
		}
		else
		{
			x_left_up = (0 - b_left) / k_left;
			x_left_bottom = (267 - b_left) / k_left;
			x_right_up = (0 - b_right) / k_right;
			x_right_bottom = (267 - b_right) / k_right;
			break;
		}
	}
	cout << round(x_left_up) << " " << round(x_left_bottom) << endl;
	cout << round(x_right_up) << " " << round(x_right_bottom) << endl;
	setdestruct();
	return;
}

int main()
{
	line_detection();
	return 0;
}
