
#include <string>
#include <sstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../build/headers/b_w.h"
#include <wiringSerial.h>
#include <fstream>

using namespace std;
using namespace cv;
using namespace raspicam;

int main()
{
	while (true)
        {
                string num;
		//cout << "ENTER IMAGE NUMBER: ";
		//cin >> num;
                string path = "../test.jpg";
                string path_mask = "../mask.jpg";
		//path_mask.insert(10, num);
		//path.insert(7, num);
                Mat frame = imread(path);
		Mat blur, mask;
		static const int arr[] = {0, 0, 0};
                vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
                static const int arr1[] = {150, 150, 150};
                vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
                bilateralFilter(frame, blur, 15, 75, 75);
                inRange(blur, lower_white, upper_white, mask);
		imwrite(path_mask, mask);
		cout << "DONE! CONTINUE? (y/n): ";
		char answer;
		cin >> answer;
		if (answer == 'n') return 0;
	}
}
