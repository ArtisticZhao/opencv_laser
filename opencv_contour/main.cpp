//answerOpenCV OpenCV / C++ - Filling holes
// #include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <vector>


using namespace cv;
using namespace std;

//find the biggest contour
vector<Point> FindBigestContour(Mat src) {
	int imax = 0;
	int imaxcontour = -1;
	std::vector<std::vector<Point> >contours;
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		int itmp = contourArea(contours[i]);
		if (imaxcontour < itmp) {
			imax = i;
			imaxcontour = itmp;
		}
	}
	return contours[imax];
}

//remove Light difference by using top hat
Mat moveLightDiff(Mat src, int radius) {
	Mat dst;
	Mat srcclone = src.clone();
	Mat mask = Mat::zeros(radius * 2, radius * 2, CV_8U);
	circle(mask, Point(radius, radius), radius, Scalar(255), -1);
	//top hat
	erode(srcclone, srcclone, mask);
	dilate(srcclone, srcclone, mask);
	dst = src - srcclone;
	return dst;
}

int main(void)
{
	Mat src = imread("e:/1.bmp");
	Mat src_hsv;
	Mat bin;
	Mat src_h;

	cvtColor(src, src_hsv, COLOR_BGR2HSV);
	vector<Mat> rgb_planes;
	split(src_hsv, rgb_planes);
	src_h = rgb_planes[0]; // h channel is useful

	src_h = moveLightDiff(src_h, 40);
	threshold(src_h, bin, 100, 255, THRESH_OTSU);

	//find and draw the biggest contour
	vector<Point> bigestcontrour = FindBigestContour(bin);
	vector<vector<Point> > controus;
	controus.push_back(bigestcontrour);
	cv::drawContours(src, controus, 0, Scalar(0, 0, 255), 3);
	imshow("1",src);
	waitKey();
	return 0;
}