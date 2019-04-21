#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace cv;

string color_now;
vector <float> coef(4);

class B_W
{
	private:

		/*
		* Returns color depends on the average value and previous color.
		*/
		string color_decode(float color_class, string previous)
		{
			if (color_class <= 0.45)
			{
				return "White";
			}
			else if (color_class > 0.45 && color_class < 0.55)
			{
				return previous;
			}
			else
			{
				return "Black";
			}
		}

		/*
		* Returns color depends on the average value.
		*/
		string color_decode_start(float color_class)
		{
			if (color_class <= 0.5)
			{
				return "White";
			}
			else
			{
				return "Black";
			}
		}

		/*
		* Searhes k and b coefficients depends on y = x * k + b line funcion and two pair of coordinates.
		*/
		vector<float> find_coefficients(int x_1, int y_1, int x_2, int y_2)
		{
			float b = (y_2 * x_1 - y_1 * x_2) / (x_1 - x_2);
			float k = 0;
			if (x_1 != 0)
			{
				k = (y_1 - b) / x_1;
			}
			else
			{
				k = (y_2 - b) / x_2;
			}
			vector<float> coefficients(2);
			coefficients[0] = k;
			coefficients[1] = b;
			return coefficients;
		}
	public:

		/*
		* Color detection.
		*/
		string color_detect_start(Mat frame)
		{
			vector<int> array(frame.rows * frame.cols);
			if (frame.isContinuous())
			{
				array.assign(frame.datastart, frame.dataend);
			}
			else
			{
				for (int i = 0; i < frame.rows; ++i)
				{
					array.insert(array.end(), frame.ptr<int>(i), frame.ptr<int>(i) + frame.cols);
				}
			}
			float value = 0;
			for (int i = 0; i < array.size(); i++)
			{
				value += array[i] / 255;
			}
			value /= array.size();
			return color_decode_start(value);
		}

		/*
		* Color and borders detection.
		* Returns color and coefficients of left and right lines (coefficients - k amd b from y = k * x + b line function)
		* in order: color, left k, left b, right k, right b.
		*/
		void color_detect(Mat frame, string previous)
		{
			vector<int> array(frame.rows * frame.cols);
			if (frame.isContinuous())
			{
				array.assign(frame.datastart, frame.dataend);
			}
			else
			{
				for (int i = 0; i < frame.rows; ++i)
				{
					array.insert(array.end(), frame.ptr<int>(i), frame.ptr<int>(i) + frame.cols);
				}
			}
			float value = 0;
			int x_1 = 0;
			int y_1 = 0;
			int x_2 = 0;
			int y_2 = 0;
			bool flag = false;

			// Searches of left line coordinates.
			int prev = array[0];
			for (int j = 0; j < frame.rows; j++)
			{
				for (int i = 0; i < frame.cols / 2; i++)
				{
					if (prev != array[j * frame.cols + i])
					{
						if (!flag)
						{
							x_1 = i;
							y_1 = j;
							x_2 = x_1;
							y_2 = y_1;
							flag = true;
						}
						else if (y_1 == j)
						{
							x_1 = i;
							y_1 = j;
							x_2 = x_1;
							y_2 = y_1;
						}
						else
						{
							x_2 = i;
							y_2 = j;
						}
						prev = array[j * frame.cols + i];
					}
					if (i >= 250)
					{
						value += array[j * frame.cols + i] / 255;
					}
				}
			}
			vector <float> coefficients(2);
			if (flag && x_1 != x_2 && y_1 != y_2)
			{
				coefficients = find_coefficients(x_1, y_1, x_2, y_2);
				coef[0] = coefficients[0];
				coef[1] = coefficients[1];
			}
			else
			{
				coef[0] = 0;
				coef[1] = 0;
			}
			cout << "LEFT: " << coef[0] << " " << coef[1] << endl;
			// Searches of right line coordinates.
			prev = array[frame.cols - 1];
			flag = false;
			x_1 = 0;
			y_1 = 0;
			x_2 = 0;
			y_2 = 0;
			for (int j = 0; j < frame.rows; j++)
			{
				for (int i = frame.cols - 1; i >= frame.cols / 2; i--)
				{
					if (prev != array[j * frame.cols + i])
					{
						if (!flag)
						{
							x_1 = i;
							y_1 = j;
							x_2 = x_1;
							y_2 = y_1;
							flag = true;
						}
						else if (y_1 == j)
						{
							x_1 = i;
							y_1 = j;
							x_2 = x_1;
							y_2 = y_1;
						}
						else
						{
							x_2 = i;
							y_2 = j;
						}
						prev = array[j * frame.cols + i];
					}
					if (i < 550)
					{
						value += array[j * frame.cols + i] / 255;
					}
				}
			}
			value /= 30000;
			color_now = color_decode(value, previous);
			if (flag && x_1 != x_2 && y_1 != y_2)
			{
				coefficients = find_coefficients(x_1, y_1, x_2, y_2);
				coef[2] = coefficients[0];
				coef[3] = coefficients[1];
			}
			else
			{
				coef[2] = 0;
				coef[3] = 0;
			}
			cout << "RIGHT: " << coef[2] << " " << coef[3] << endl;
		}

		string get_color()
		{
			return color_now;
		}

		vector <float> get_coefficients()
		{
			return coef;
		}

		/*
		* Color detection that depends on size of frame (x, y, width, height)
		*/
		string mid_detect(Mat frame, int x, int y, int w, int h)
		{
			Rect roi(x, y, w, h);
			Mat crop_image = frame(roi);
			Mat blur;
		//	bilateralFilter(crop_image, blur, 15, 75, 75);
			static const int arr[] = {0, 0, 0};
			vector<int> lower_white (arr, arr + sizeof(arr) / sizeof(arr[0]) );
			static const int arr1[] = {110, 110, 110};
			vector<int> upper_white (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
			Mat mask;
			inRange(crop_image, lower_white, upper_white, mask);
			vector<int> mask_arr;
			if (mask.isContinuous())
			{
				mask_arr.assign(mask.datastart, mask.dataend);
			}
			else
			{
				for (int i = 0; i < mask.rows; ++i)
				{
					mask_arr.insert(mask_arr.end(), mask.ptr<int>(i), mask.ptr<int>(i)+mask.cols);
				}
			}
			float med = 0;
			for (int i = 0; i < mask_arr.size(); i++)
			{
				med +=  mask_arr[i] / 255;
			}
			med /= mask_arr.size();
			return color_decode_start(med);
		}
};
