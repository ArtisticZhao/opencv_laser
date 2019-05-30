#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
#include "point_detector.h"

//#define  _DEBUG_H_

#ifdef _DEBUG_H_
Mat g_h;
Mat g_s;
Mat g_v;

static void _mouseCallback(int event, int x, int y, int flags, void *userdata)
{
	if (EVENT_LBUTTONDOWN == event) {

		printf("x:%d, y:%d, h:%hhu, s:%hhu, v:%hhu\r\n",
			x, y,
			g_h.at<unsigned char>(y, x),
			g_s.at<unsigned char>(y, x),
			g_v.at<unsigned char>(y, x));
	}
}
#endif

//判断边框区域是否是圆形
static bool _isRound(vector<Point> &contour)
{
	double perimeter, area, area_c;

	perimeter = arcLength(contour, true);
	area = contourArea(contour, false);
	area_c = (perimeter * perimeter) / (4 * 3.1415);

	if (area_c / area < 1.5) {
		return true;
	}
	else {
		return false;
	}
}

/*
	1、高斯模糊
	2、拆分HSV通道
	3、判断三个通道的数值范围选出图像进行与操作
	4、腐蚀掉小范围的区域，
	5、估计剩余为圆形的区域
*/
int PointDetector::Detect(Mat &src, vector<Point2d> &points)
{
	Mat temp;
	Mat kernel;
	Mat h, s, v;
	vector<Mat> channels;
	vector<vector<Point> > contours;

	//1、高斯模糊
	GaussianBlur(src, temp, Size(7, 7), 0, 0);

	//2、拆分HSV通道
	cvtColor(temp, temp, COLOR_BGR2HSV);
	split(temp, channels);
	h = channels.at(0);
	s = channels.at(1);
	v = channels.at(2);

#ifdef _DEBUG_H_
	g_h = h;
	g_s = s;
	g_v = v;
	imshow("origin", src);
	imshow("h", h);
	imshow("s", s);
	imshow("v", v);
	setMouseCallback("origin", _mouseCallback, NULL);
	waitKey();
#endif

	//三通道分别进行阈值化
	threshold(h, h, 167, 255, THRESH_BINARY);
	threshold(s, s, 40, 255, THRESH_BINARY);
	threshold(v, v, 180, 255, THRESH_BINARY);

	bitwise_and(h, s, temp);
	bitwise_and(v, temp, temp);

	//形态学
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(temp, temp, MORPH_CLOSE, kernel);
	morphologyEx(temp, temp, MORPH_OPEN, kernel);

#ifdef _DEBUG_H_
	imshow("h_thresh", h);
	imshow("s_thresh", s);
	imshow("v_thresh", v);
	imshow("openclose", temp);
	waitKey();
#endif

	//查找边缘
	findContours(temp, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//遍历边缘
	for (int i = 0; i < contours.size(); ++i) {
		bool ok;
		Point2d point;
		Moments m;

		//判断是否是圆形
		ok = _isRound(contours[i]);
		if (true != ok) {
			continue;
		}

		//画出所选区域
		//drawContours(src, contours, i, Scalar(0, 255, 0));

		//计算光点位置
		m = moments(contours[i], true);
		point.x = m.m10 / m.m00;
		point.y = m.m01 / m.m00;
		points.push_back(point);
	}

	return (int)points.size();
}
