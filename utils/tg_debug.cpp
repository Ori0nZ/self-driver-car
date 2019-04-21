#include <string>
#include <sstream>
#include "line_reco.h"
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace raspicam;

Line *line_reco = new Line;
RaspiCam_Cv Picamera;

void setup()
{
	Picamera.set(CV_CAP_PROP_FRAME_WIDTH, 800); // width = 800
	Picamera.set(CV_CAP_PROP_FRAME_HEIGHT, 400); // height = 400
	Picamera.set(CV_CAP_PROP_FORMAT, CV_8UC3); // 3 chanels
	Picamera.set(CV_CAP_PROP_EXPOSURE, -1); // exposure off (default is -1, so this can be removed)
	Picamera.set(CV_CAP_PROP_GAIN,100); // iso = 100 (you can choose values [-100,100] include borders
	cout << "Opening video stream\n";
	if (!Picamera.isOpened())
	{
		if ( !Picamera.open())
		{
			cout << "Unable to open\n";
		}
	}
}

void setdestruct()
{
	cout << "Closing video stream\n";
	Picamera.release();
}


int main()
{
	setup();
	int n = 0;
	while (n < 1000)
	{
		Picamera.grab();
		Mat origframe;
		Picamera.retrieve(origframe);
		int iImageHieght = origframe.rows / 2;
		int iImageWidth = origframe.cols / 2;
		Mat matRotation = getRotationMatrix2D( Point(iImageWidth, iImageHieght), 180, 1 );
		Mat frame;
		warpAffine(origframe, frame, matRotation, origframe.size());
		if (!frame.empty())
		{
			n++;
			stringstream ss;
			string path = "frames/.jpg";
			ss << n;
			path.insert(7, ss.str());
			ss.str("");
			imwrite(path, frame);
			line_reco->starting(frame);
			vector< vector<int> > l_line = line_reco->answer.l_line;
			vector< vector<int> > r_line = line_reco->answer.r_line;
			float tg = line_reco->angle_calc(r_line, l_line);
			cout << "Tangens: " << tg << " || Frame path: " << path << endl;
		}

	}
	cout << "1000 frames are done!\n";
	setdestruct();
	return 0;
}
