#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;


int main()
{
    Mat input = imread("frames/1.jpg");
    //line (input, Point(174,0), Point(16,197), Scalar(0,0,255));
    //line (input, Point(566,0), Point(726,174), Scalar(0,0,255));
    //imshow("Input", input);

    Point2f inputQuad[4];
    inputQuad[0] = Point2f( 174, 0 );
    inputQuad[1] = Point2f( 16,197 );
    inputQuad[2] = Point2f( 566, 0 );
    inputQuad[3] = Point2f( 726, 174 );

    Point2f outputQuad[4];
    outputQuad[0] = Point2f( 16, 0 );
    outputQuad[1] = Point2f( 16, 297 );
    outputQuad[2] = Point2f( 726, 0 );
    outputQuad[3] = Point2f( 726, 274 );

    Mat M = getPerspectiveTransform( inputQuad, outputQuad );
/*
    vector<Point2f> inputCorners(4);
    inputCorners[0]=Point2f(0, 0);
    inputCorners[1]=Point2f(input.cols, 0);
    inputCorners[2]=Point2f(0, input.rows);
    inputCorners[3]=Point2f(input.cols, input.rows);

    vector<Point2f> outputCorners(4);
    perspectiveTransform(inputCorners, outputCorners, M);

    Rect br= boundingRect(outputCorners);

    for(int i=0; i<4; i++) {
        outputQuad[i]+=Point2f(-br.x,-br.y);
    }
*/
    M = getPerspectiveTransform( inputQuad, outputQuad );
    Mat output, blur, b_w;
    warpPerspective(input, output, M, input.size());
    /*bilateralFilter(output, blur, 15, 75, 75);
    static const int arr[] = {0, 0, 0};
    vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    static const int arr1[] = {150, 150, 150};
    vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
    inRange(blur, lower_white, upper_white, b_w);*/
    //rectangle (output, Point(0,0), Point(800,100), Scalar(0,0,255));
    //rectangle (output, Point(0,200), Point(800,300), Scalar(0,0,255));
    //resize(input, input, Size(1000,1000));
    //imshow("Input", input);
    //resize(output, output, Size(1000,1000));
    imshow("Perspective", output);
    //imshow("BW", b_w);

	waitKey(0);
	return 0;
}