#include "b_w.h"
#include "line_reco.h"
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

using namespace std;
using namespace cv;
using namespace raspicam;

B_W *b_w = new B_W;
Line *line_reco = new Line;
RaspiCam Picamera;
string color;
int frame_num;

class Test_Cam
{
	private:

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
		* Sends command to arduino.
		*/
		void sendcmd(int serial_port, string str)
		{
			int i = 0;
			cout << "Command: " << str << endl;
			while(1)
			{
				serialPutchar(serial_port, str[i]);
				fflush(stdout);
				if (str[i] == '!') return;
				i++;
			}
		}

		void move_reverse(int serial_port)
		{
			setup();
                        while (1)
                        {
                                Picamera.grab();
                                unsigned char *data=new unsigned char[Picamera.getImageTypeSize(RASPICAM_FORMAT_BGR)];
                                Picamera.retrieve (data);
                                Mat frame(400, 800, CV_8UC3);
                                memcpy(frame.data, data, 400*800*3*sizeof(unsigned char));
                                delete data;
                                if (!frame.empty())
                                {
					string up = b_w->mid_detect(frame, 350, 300, 100, 50);
					string bottom = b_w->mid_detect(frame, 350, 350, 100, 50);
					if (up == bottom && bottom != color)
					{
						cout << "THE NEXT SQUARE IS TOO CLOSE!\n";
						sendcmd(serial_port, "ZC+570!");
						sendcmd(serial_port, "LC+560!");
						sendcmd(serial_port, "FWD!");
					}
					else
					{
						sendcmd(serial_port, "STP!");
						cout << "DISTANCE TO THE NEXT SQUARE IS NORMAL!\n";
						break;
					}
				}
				else
				{
					sendcmd(serial_port, "STP!");
					cout << "ERROR: Can not get frame!\n";
				}
usleep(500000);
			}
			setdestruct();

		}
		/*
		* Turn function.
		*/
		void corner_stop(int serial_port, string direction)
		{
			setup();
			int counter = 0;
			int n;
			bool flag = false;
			//string bottom = "White";
			frame_num = 0;
			stringstream ss;
			string bottom = color;
			if (direction == "left") sendcmd (serial_port, "LFT!");
			else sendcmd (serial_port, "RGT!");
			usleep(500000);
			time_t t1 = time(0);
			while (1)
			{
				n++;
				Picamera.grab();
				unsigned char *data=new unsigned char[Picamera.getImageTypeSize(RASPICAM_FORMAT_BGR)];
				Picamera.retrieve (data);
				Mat frame(400, 800, CV_8UC3);
				memcpy(frame.data, data, 400*800*3*sizeof(unsigned char));
				delete data;
/*
				string path = "/home/pi/frames/.jpg";
                                frame_num++;
                                ss << frame_num;
                                path.insert(16, ss.str());
                                ss.str("");
				rectangle(frame, Point(0, 70), Point(50,120), Scalar (255,255,0), 2);
				rectangle(frame, Point(750, 70), Point(800,120), Scalar (255,255,0), 2);
				rectangle(frame, Point(350, 70), Point(450,120), Scalar (255,255,0), 2);
*/
				string mid = b_w->mid_detect(frame, 350, 100, 100, 50);
				string rgt = b_w->mid_detect(frame, 750, 100, 50, 50);
				string lft = b_w->mid_detect(frame, 0, 100, 50, 50);
/*
				putText(frame, lft, Point (50,300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
                               	putText(frame, rgt, Point (600,300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
				putText(frame, mid, Point (400,300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
				imwrite (path, frame);
                                cout << "FRAME WRITTEN: " << path << endl;
*/
				cout << n <<  " ~~~ UP: " << mid << "; BOTTOM: " << bottom <<"; RGT: " << rgt << "; LFT: " << lft << endl;
				time_t t2 = time(0);
				if (mid == bottom)
				{
					counter++;
					if (bottom == "Black") bottom = "White";
					else bottom = "Black";
				}

				if (counter >= 1)
				{
					if (direction == "left")
					{
						if (mid == rgt && counter == 2)
						{
							cout << "CORNER BY ALGORITHM\n";
							sendcmd(serial_port, "STP!");
							break;
						}
						else if (counter > 2 || t2 >= t1 + 4)
	                                        {
	                                                cout << "CORNER BY TIME\n";
	                                                //usleep(300000);
	                                                sendcmd(serial_port, "STP!");
	                                                break;
						}
					}
					else
					{
                                                if (mid == lft && counter == 2)
                                                {
                                                        cout << "CORNER BY ALGORITHM\n";
                                                        sendcmd(serial_port, "STP!");
                                                        break;
                                                }
                                                else if (counter > 2 || t2 >= t1 + 4)
                                                {
                                                        cout << "CORNER BY TIME\n";
                                                        //usleep(300000);
                                                        sendcmd(serial_port, "STP!");
                                                        break;
                                                }
                                        }

				}
usleep(200000);
			}
			setdestruct();
			move_reverse(serial_port);
		}

		/*
		* Move forward. It count squares and correct loacation, depends on squares boards.
		*/
		void FWD_correction (int serial_port, int zxc)
		{
			setup();
			string previous = color;
			string prev_command;
			int n = 0, count_qrd = 0;
			stringstream ss;

			while (1)
			{
				Picamera.grab();
				unsigned char *data=new unsigned char[Picamera.getImageTypeSize(RASPICAM_FORMAT_BGR)];
				Picamera.retrieve (data);
				Mat frame(400, 800, CV_8UC3);
				memcpy(frame.data, data, 400*800*3*sizeof(unsigned char));
				delete data;
/*
				string path = "/home/pi/frames/.jpg";
				frame_num++;
				ss << frame_num;
				path.insert(16, ss.str());
				ss.str("");
				//imwrite (path, frame);
				//cout << "FRAME WRITTEN: " << path << endl;
*/
				if (!frame.empty())
				{
					if (previous == "")
					{
						// If it first time when camera was setuped,
						// it's only need to find color
						Rect roi_bottom(0, frame.size().height - 50, frame.size().width, 50);
						Mat frame_bottom = frame(roi_bottom);
						Mat blur_bottom, mask_bottom;
					//	bilateralFilter(frame_bottom, blur_bottom, 15, 75, 75);
						static const int arr[] = {0, 0, 0};
						vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
						static const int arr1[] = {110, 110, 110};
						vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
						inRange(frame_bottom, lower_white, upper_white, mask_bottom);
						previous = b_w->color_detect_start(mask_bottom);
						cout << "FIRST: " << previous << endl;
					}
					else
					{
						Mat blur_up, blur_bottom, mask_up, mask_bottom;
						Rect roi_up(0, 50, frame.size().width, 50);
						Rect roi_bottom(0, 300, frame.size().width, 100);
						Mat frame_up = frame(roi_up);
						Mat frame_bottom = frame(roi_bottom);
						static const int arr[] = {0, 0, 0};
						vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
						static const int arr1[] = {110, 110, 110};
						vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
					//	bilateralFilter(frame_up, blur_up, 15, 75, 75);
					//	bilateralFilter(frame_bottom, blur_bottom, 15, 75, 75);
						inRange(frame_up, lower_white, upper_white, mask_up);
						inRange(frame_bottom, lower_white, upper_white, mask_bottom);
						vector <float> answer_up(4);
						vector <float> answer_bottom(4);
						cout << "||||BOTTOM||||\n";
						cout << "vvvvvvvvvvvvvv\n";
						b_w->color_detect(mask_bottom, previous);
						string info = b_w->get_color();
						answer_bottom = b_w->get_coefficients();
						cout << "||||  UP  ||||\n";
                                                cout << "vvvvvvvvvvvvvv\n";
						b_w->color_detect(mask_up, previous);
						string mid = b_w->get_color();
						answer_up = b_w->get_coefficients();
						cout << "PREVIOUS: " << previous << " || NOW: " << info << " || NEXT: " << mid << endl;;
						answer_up[1] += 50;
                                                answer_up[3] += 50;
						answer_bottom[1] += 300;
						answer_bottom[3] += 300;
/*						vector<int> ideal(4);
						ideal = line_reco->get_ideal_line();
						line(frame, Point(ideal[0], 0), Point(ideal[1], 267), Scalar(255,255,0), 4);
						line(frame, Point(ideal[2], 0), Point(ideal[3], 267), Scalar(255,255,0), 4);
						line(frame, Point((50 - answer_up[1]) / answer_up[0], 50), Point((100 - answer_up[1]) / answer_up[0], 100), Scalar(0,255,255), 4);
						line(frame, Point((50 - answer_up[3]) / answer_up[2], 50), Point((100 - answer_up[3]) / answer_up[2], 100), Scalar(0,255,255), 4);
*/						if (previous != info)
						{
							count_qrd++;
							cout << "                                                                                                   SQUARE " << count_qrd << endl;
						}

						sendcmd(serial_port, "ZC+380!");
						sendcmd(serial_port, "RC+390!");

						if (count_qrd >= zxc)
						{
							/*if (mid != info )
							{
								cout << "STOP BY ALGORITHM\n";
								sendcmd(serial_port, "STP!");
								sendcmd(serial_port, "ZC!");
								setdestruct();
								color = info;
								//move_reverse(serial_port);
								return;
							}
							else*/ if (count_qrd > zxc )
							{
								cout << "STOP BY TIME\n";
								sendcmd(serial_port, "STP!");
								sendcmd(serial_port, "ZC!");
								setdestruct();
								color = previous;
								line_reco->reset_directions();
								move_reverse(serial_port);
								return;
							}
						}
						previous = info;

						// Correction.
						vector <float> left_up(2);
						vector <float> left_bottom(2);
						vector <float> right_up(2);
						vector <float> right_bottom(2);
						left_up[0] = answer_up[0];
						left_up[1] = answer_up[1];
						right_up[0] = answer_up[2];
						right_up[1] = answer_up[3];
						left_bottom[0] = answer_bottom[0];
						left_bottom[1] = answer_bottom[1];
						right_bottom[0] = answer_bottom[2];
						right_bottom[1] = answer_bottom[3];
						string command = line_reco->correction(left_up, left_bottom, right_up, right_bottom, 0, 267);

						cout << command << " CORRECTION" << endl;
/*						putText(frame, command, Point (300,300), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
						imwrite (path, frame);
		                                cout << "FRAME WRITTEN: " << path << endl;
*/
						if (command == "LEFT")
						{
							sendcmd(serial_port, "LC+400!");
						}
						else if (command == "RIGHT")
						{
							sendcmd(serial_port, "RC+410!");
						}
						else
						{

							sendcmd(serial_port, "ZC+380!");
							sendcmd(serial_port, "RC+390!");
						}
						sendcmd(serial_port, "FWD!");
					}
				}
				else
				{
					sendcmd(serial_port, "STP!");
					cout << "ERROR: Can not get frame!\n";
				}
usleep(200000);
			}
		}

	public:

		void left (int serial_port)
		{
			corner_stop(serial_port, "left");
			usleep (1000000);
		}

		void right (int serial_port)
		{
			corner_stop(serial_port, "right");
			usleep (1000000);
		}

		void forward (int serial_port, int count)
		{
			FWD_correction (serial_port, count);
			usleep (1000000);
		}

		void line_detection ()
		{
			setup();
			while(1)
			{
				Picamera.grab();
                                        unsigned char *data=new unsigned char[Picamera.getImageTypeSize(RASPICAM_FORMAT_BGR)];
                                        Picamera.retrieve (data);
                                        Mat frame(400, 800, CV_8UC3);
                                        memcpy(frame.data, data, 400*800*3*sizeof(unsigned char));
                                        delete data;

				if (!frame.empty())
				{
					Mat blur, mask;
					Rect roi(0, 50, frame.size().width, 50);
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
					coefficients[1] += 50;
					coefficients[3] += 50;

					float k_left = coefficients[0];
					float b_left = coefficients[1];
					float k_right = coefficients[2];
					float b_right = coefficients[3];

					float x_left_up = (0 - b_left) / k_left;
					float x_left_bottom = (267 - b_left) / k_left;
					float x_right_up = (0 - b_right) / k_right;
					float x_right_bottom = (267 - b_right) / k_right;
					cout << "LEFT X: " << x_left_up << " && " << x_left_bottom << endl;
					cout << "RIGHT X: " << x_right_up << " && " << x_right_bottom << endl;
					if (k_left > -0.01 || k_right < 0.01 || x_left_up > 399 || x_right_up < 400 )
					{
						cout << "LINE WAS NOT FOUND! MOVE ME!\n";
					}
					else
					{
						cout << "LINE WAS FOUND!\n";
						vector <int> line(4);
						line[0] = round(x_left_up);
						line[1] = round(x_left_bottom);
						line[2] = round(x_right_up);
						line[3] = round(x_right_bottom);

						line_reco->set_ideal_line(line);

						usleep(1000000);
						break;
					}
				}
				else
				{
					cout << "ERROR: Can not get frame!\n";
				}
			}
			setdestruct();
			return;
		}
};
