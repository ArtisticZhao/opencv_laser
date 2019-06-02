#include "my_point_detector.h"

void get_point(Mat& origin_light, Mat& origin_dark) {
	// to gray img
	Mat origin_light_gray;
	cvtColor(origin_light, origin_light_gray, COLOR_RGB2GRAY);
	Mat origin_dark_gray;
	cvtColor(origin_dark, origin_dark_gray, COLOR_RGB2GRAY);
	Mat sub_gray;
	sub_gray = origin_light_gray - origin_light_gray;
	// debug
	imshow("ԭͼ1", sub_gray);
}