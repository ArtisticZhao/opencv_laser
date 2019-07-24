#include "my_point_detector.h"
//#define  _DEBUG_H_
using namespace std;
using namespace cv;
int Histogram(Mat* grayImage);
#ifdef _DEBUG_H_
Mat gray;
static void _mouseCallback(int event, int x, int y, int flags, void* userdata)
{
	if (EVENT_LBUTTONDOWN == event) {
		printf("x:%d, y:%d, val:%d\r\n",
			x, y,
			gray.at<unsigned char>(y, x));
	
	}
}
#endif
//判断边框区域是否是圆形
bool isRoundcontour(vector<Point>& contour){
	double perimeter, area, area_c;
	perimeter = arcLength(contour, true);
	area = contourArea(contour, false);
	area_c = (perimeter * perimeter) / (4 * 3.1415);
	if (area_c / area < 2) {
		return true;
	}else {
		//cout << "area_c / area = " << area_c / area << endl;
		return false;
	}
}
void get_point(Mat& origin_light, Mat& origin_dark, vector<Point2d>& points) {
	// to gray img
	Mat origin_light_gray;
	cvtColor(origin_light, origin_light_gray, COLOR_RGB2GRAY);
	Mat origin_dark_gray;
	cvtColor(origin_dark, origin_dark_gray, COLOR_RGB2GRAY);
	Mat sub_gray;
	subtract(origin_light_gray, origin_dark_gray, sub_gray);
	namedWindow("sub", WINDOW_NORMAL);
	namedWindow("threshold", WINDOW_NORMAL);
	namedWindow("fliter", WINDOW_NORMAL);
	imshow("sub", sub_gray);
	//形态学
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	// 确定二值化阈值
	int thres = Histogram(&sub_gray);
	// 二值化
	threshold(sub_gray, sub_gray, thres, 255, THRESH_BINARY);
	imshow("threshold", sub_gray);
	//形态学
	morphologyEx(sub_gray, sub_gray, MORPH_CLOSE, kernel);
	morphologyEx(sub_gray, sub_gray, MORPH_OPEN, kernel);
	//namedWindow("beforesub", WINDOW_NORMAL);
	//namedWindow("sub", WINDOW_NORMAL);
	//imshow("beforesub", sub_gray);
	
	imshow("fliter", sub_gray);
	// debug
	//imshow("sub", sub_gray);
#ifdef _DEBUG_H_
	gray = sub_gray;
	setMouseCallback("sub", _mouseCallback, NULL);
	waitKey();
#endif
	//查找边缘
	vector<vector<Point> > contours;
	findContours(sub_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	int max = 0;
	int max_index=0;
	//遍历边缘
	for (int i = 0; i < contours.size(); ++i) {
		if (contours.at(i).size() > max) {
			max = contours.at(i).size();
			max_index = i;
		}
		//判断是否是圆形
		/*ok = isRoundcontour(contours[i]);
		if (true != ok) {
			continue;
		}*/
	}
	if (contours.size() > 0) {
		//计算光点位置
		Point2d point;
		Moments m;
		m = moments(contours[max_index], true);
		point.x = m.m10 / m.m00;
		point.y = m.m01 / m.m00;
		points.push_back(point);
	}
}

int Histogram(Mat* grayImage) {
	cv::MatND histogram;
	//256个，范围是0，255.
	const int histSize = 256;
	float range[] = { 0, 255 };
	const float* ranges[] = { range };
	const int channels = 0;

	cv::calcHist(grayImage, 1, &channels, cv::Mat(), histogram, 1, &histSize, &ranges[0], true, false);
	int row = histogram.rows;
	int col = histogram.cols;

	float* h = (float*)histogram.data;
	if (h) {
		for (int i = 0; i < 256; ++i) {
			if (h[i] < 40) {
				return i;
			}
		}
	}
	cout << endl;
}