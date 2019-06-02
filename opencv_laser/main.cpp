
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <core/types.hpp>

#include "point_detector.h"
#include "triangle.h"
void test_point(Mat &origin);
static void on_mouse(int event, int x, int y, int flags, void* ustc);
using namespace cv;
using namespace std;
int width = 0;
int height = 0;
int main()
{
	VideoCapture capture(2);
	// 通过下面两行设置像素分辨率, 设定值如果超过
	capture.set(CAP_PROP_FRAME_WIDTH, 5000);
	capture.set(CAP_PROP_FRAME_HEIGHT, 5000);
	namedWindow("读取视频", 1);
	int key = 0;
	setMouseCallback("读取视频", on_mouse, NULL);
	int flag = 0;
	while (key != 'q')
	{
		Mat frame;
		capture >> frame;
		if (flag == 0) {
			flag = 1;
			width = frame.size().width;
			height = frame.size().height;
		}
		test_point(frame);
		// draw cross
		line(frame, Point2d(0, height / 2), Point2d(width, height / 2), Scalar(0, 255, 255), 1, LINE_AA);
		line(frame, Point2d(width / 2, 0), Point2d(width / 2, height), Scalar(0, 255, 255), 1, LINE_AA);
		imshow("读取视频", frame);
		key = waitKey(1);	//延时30
	}
	destroyAllWindows();
	return 0;
}

void test_point(Mat &origin)
{
	vector<Point2d> points;
	PointDetector pd;
	//检测点
	pd.Detect(origin, points);

	//遍历边缘
	if (points.size() == 1) {
		// measure
		double d;
		//画出所选区域
		cv::circle(origin, points[0], 5, Scalar(0, 255, 0));
		cout << points[0].x- origin.size().width / 2 << " " << points[0].y - origin.size().height/2 << endl;
		//f = measuref(points[0].x, points[0].y, 28, 46, 90);
		d = triangle(points[0].x, points[0].y, 90, 28, 0.0277);
		cout << "d is " << d << endl;
		return;
	}
	else {
		return;
	}
	//for (int i = 0; i < points.size(); ++i) {

	//	//画出所选区域
	//	cv::circle(origin, points[i], 5, Scalar(0, 255, 0));
	//	cout << points[i].x << " " << points[i].y << endl;
	//}
	//return;
	//imshow("origin", origin);

	//waitKey();
}

static void on_mouse(int event, int x, int y, int flags, void* ustc) {
	if (event == EVENT_LBUTTONDOWN)//鼠标左键按下事件发生  
	{
		//printf( "(%d,%d)", x-width, y-height);//打印当前坐标值
		cout << "(" << x - width/2 << "," << y - height/2 << ")" << endl;
	}

}