#pragma once
#include <opencv2/opencv.hpp>
#include <core/types.hpp>
#include <vector>
using namespace cv;
using namespace std;
void get_point(Mat& origin_light, Mat& origin_dark, vector<Point2d>& points);
