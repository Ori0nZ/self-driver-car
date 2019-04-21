#include <string>
#include <sstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "b_w.h"
#include <wiringSerial.h>
#include <fstream>

using namespace std;
using namespace cv;
using namespace raspicam;

int main()
{
	ifstream fin("line_logs.txt");
	if (!fin)
	{
		cout << "ERROR!\n";
		return 0;
	}
	else
	{
		cout << "FILE IS OPENED!\n";
	}
	float x_1, x_2, x_3, x_4;
	int n = 0;
	string l, r, d;
	stringstream ss;
	while (!fin.eof())
	{
		fin >> x_1 >> x_2 >> x_3 >> x_4;
		fin >> l;
		fin >> r;
		fin >> d;
		n++;
		string path = "line_frames/.jpg";
                ss << n;
	        path.insert(12, ss.str());
                ss.str("");
		Mat frame = imread(path);
		//if (x_1 != 0 && x_2 != 0) line (frame, Point(x_1,200), Point(x_2,300), Scalar(0,255,255));
		//if (x_3 != 0 && x_4 != 0) line (frame, Point(x_3,200), Point(x_4,300), Scalar(0,255,255));
		circle (frame, Point(x_1,200), 5, Scalar(0,255,255), 3);
		circle (frame, Point(x_2,250), 5, Scalar(0,255,255), 3);
		circle (frame, Point(x_3,200), 5, Scalar(0,255,255), 3);
		circle (frame, Point(x_4,250), 5, Scalar(0,255,255), 3);
		line (frame, Point(165,0), Point(-45,267), Scalar(255,0,0));
                line (frame, Point(620,0), Point(820,267), Scalar(255,0,0));
		putText (frame, l, Point (100,100), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar (0,0,255));
		putText (frame, r, Point (600,100), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar (0,0,255));
		putText (frame, d, Point (300,200), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar (0,0,255));
		imwrite (path, frame);
		cout << path << " is REWRITTEN!\n";
	}
	cout << "ALL DONE!\n";
	return 0;
}
