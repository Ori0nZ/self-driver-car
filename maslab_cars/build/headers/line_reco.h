#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <cstddef>


using namespace std;
using namespace cv;

vector <int> ideal_lines(4);
string left_previous;
string right_previous;
class Line
{
	private:

		/*
		* Detect direction for correction depends on left line.
		* Cases depends on "perfect" left line with coordinates (195;0), (0;267).
		* If it all clear, returns "FORWARD".
		*/
		string left_side(vector<float> coordinates)
		{
			float x_1 = ideal_lines[0];
			float x_2 = ideal_lines[1];
			float delta = 50;
			if (x_1 > coordinates[0] && x_2 > coordinates[1] && ((x_1 - coordinates[0]) > delta || (x_2 - coordinates[1]) > delta) || x_1 > coordinates[0] && x_2 < coordinates[1] && ((x_1 - coordinates[0]) > delta || (coordinates[1] - x_2) > delta ))
			{
				return "LEFT";
			}
			else if (x_1 < coordinates[0] && x_2 < coordinates[1] && ((coordinates[0] - x_1) > delta || (coordinates[1] - x_2) > delta) || x_1 < coordinates[0] && x_2 > coordinates[1] && ((coordinates[0] - x_1) > delta || (x_2 - coordinates[1]) > delta ))
			{
				return "RIGHT";
			}
			else
			{
				return "FORWARD";
			}
		}

		/*
		* Detect direction for correction depends on right line.
		* Cases depends on "perfect" right line with coordinates (605;0), (800;267).
		* If it all clear, returns "FORWARD".
		*/
		string right_side (vector<float> coordinates)
		{
			float x_1 = ideal_lines[2];
			float x_2 = ideal_lines[3];
			float delta = 50;
			if (x_1 > coordinates[0] && x_2 > coordinates[1] && ((x_1 - coordinates[0]) > delta || (x_2 - coordinates[1]) > delta) || x_1 > coordinates[0] && x_2 < coordinates[1] && ((x_1 - coordinates[0]) > delta || (coordinates[1] - x_2) > delta ))
			{
				return "LEFT";
			}
			else if (x_1 < coordinates[0] && x_2 < coordinates[1] && ((coordinates[0] - x_1) > delta || (coordinates[1] - x_2) > delta) || x_1 < coordinates[0] && x_2 > coordinates[1] && ((coordinates[0] - x_1) > delta || (x_2 - coordinates[1]) > delta ))
			{
				return "RIGHT";
			}
			else
			{
				return "FORWARD";
			}
		}

		/*
		* Detection of correction direction depends on directions for left and right sides.
		*/
		string prediction (string dec1, string dec2)
		{
			if (dec1 == dec2 && dec1 != "BLANK") return dec2;
			else if (dec1 != dec2 && dec2 == "BLANK") return dec1;
			else if (dec1 != dec2 && dec1 == "BLANK") return dec2;
			else return "FORWARD";
		}

	public:

		/*
		* Seraches average of left an right lines coefficients.
		* Then detect directions for correction.
		*/
		string correction (vector<float> left_up, vector<float> left_bottom, vector<float> right_up, vector<float> right_bottom, int y_up, int y_bottom)
		{
			float k_up_left, k_up_right, b_up_left, b_up_right, k_bottom_left, k_bottom_right, b_bottom_left, b_bottom_right;
			k_up_left = left_up[0];
			k_up_right = right_up[0];
			b_up_left = left_up[1];
			b_up_right = right_up[1];
			k_bottom_left = left_bottom[0];
                        k_bottom_right = right_bottom[0];
                        b_bottom_left = left_bottom[1];
                        b_bottom_right = right_bottom[1];

			//string left_previous;
			//string right_previous;
/*
			// Calculate k of left line.
			if (left_up[0] == 0)
			{
				k_left = left_bottom[0];
			}
			else if (left_bottom[0] == 0)
			{
				k_left = left_up[0];
			}
			else
			{
				k_left = (left_up[0] + left_bottom[0]) / 2;
			}

			// Calculate b of left line.
			if (left_up[1] == 0)
			{
				b_left = left_bottom[1];
			}
			else if (left_bottom[1] == 0)
			{
				b_left = left_up[1];
			}
			else
			{
				b_left = (left_up[1] + left_bottom[1]) / 2;
			}

			// Calculate k of rightt line.
			if (right_up[0] == 0)
			{
				k_right = right_bottom[0];
			}
			else if (right_bottom[0] == 0)
			{
				k_right = right_up[0];
			}
			else
			{
				k_right = (right_up[0] + right_bottom[0]) / 2;
			}

			// Calculate b of rightt line.
			if (right_up[1] == 0)
			{
				b_right = right_bottom[1];
			}
			else if (right_bottom[1] == 0)
			{
				b_right = right_up[1];
			}
			else
			{
				b_right = (right_up[1] + right_bottom[1]) / 2;
			}
*/
			if (abs(k_bottom_left) > 0.3 && abs(k_bottom_left) < 10 && abs(k_bottom_right) <= 0.3)
			{
				cout << "==========================================HARD RIGHT CORRECTION\n";
				return "RIGHT";
			}
			if (abs(k_bottom_right) > 0.3 && abs(k_bottom_right) < 10 && abs(k_bottom_left) <= 0.3)
                        {
                                cout << "==========================================HARD LEFT CORRECTION\n";
                                return "LEFT";
                        }
return "FORWARD"; 
/*
			vector<float> left_line(2);
			vector<float> right_line(2);
			string left_prediction, right_prediction;

			// Defenition of left side correction command.
			if (abs(k_up_left) > 0.1)
			{
				left_line[0] = (y_up - b_up_left) / k_up_left;
				left_line[1] = (y_bottom - b_up_left) / k_up_left;
				cout << "LEFT: " <<  left_line[0] << " " <<  left_line[1] << endl;
				if (left_line[0] > 399 || left_line[1] > 399)
				{
					left_prediction = "BLANK";
					//left_prediction = left_previous;
					//k_left = 0;
				}
				else
				{
					left_prediction = left_side(left_line);
				}
			}
			else
			{
				left_prediction = "BLANK";
				//left_prediction = left_previous;
				//k_left = 0;
			}

			// Defenition of right side correction command.
			if (abs(k_up_right) > 0.1)
			{
				right_line[0] = (y_up - b_up_right) / k_up_right;
				right_line[1] = (y_bottom - b_up_right) / k_up_right;
				cout <<  right_line[0] << " " << right_line[1] << endl;
				if (right_line[0] < 400 || right_line[1] < 400)
				{
					right_prediction = "BLANK";
					//right_prediction = right_previous;
					//k_right = 0;
				}
				else
				{
					right_prediction = right_side(right_line);
				}
			}
			else
			{
				right_prediction = "BLANK";
				//right_prediction = right_previous;
				//k_right = 0;
			}

			//left_previous = left_prediction;
			//right_previous = right_prediction;

			// Defenition of end corection command.
			cout << "LEFT: " << left_prediction << "   RIGHT: " << right_prediction << endl;

			if (left_prediction == "RIGHT" && right_prediction == "LEFT" || left_prediction == "LEFT" && right_prediction == "RIGHT")
			{
				if (abs(k_up_left) > abs(k_up_right)) return left_prediction;
				else if (abs(k_up_left) < abs(k_up_right)) return right_prediction;
				else return "BLANK";
			}
			else
			{
				return prediction(left_prediction, right_prediction);
			}*/
		}


		void reset_directions()
		{
			left_previous = "";
			right_previous = "";
		}

		void set_ideal_line(vector <int> coordinates)
		{
			ideal_lines = coordinates;
			cout << "SET LEFT: " << coordinates[0] << " " << coordinates[1] << endl;
			cout << "SET RIGHT: " << coordinates[2] << " " << coordinates[3] << endl;
		}
		vector<int>  get_ideal_line()
		{
			return  ideal_lines;
		}
};

