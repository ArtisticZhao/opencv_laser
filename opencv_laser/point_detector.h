#ifndef _POINT_DETECTOR_H_
#define _POINT_DETECTOR_H_
#include <opencv2/opencv.hpp>
#include <core/types.hpp>
using namespace cv;
using namespace std;

class PointDetector
{
public:
	int Detect(Mat &src, vector<Point2d> &points);
};

#endif

