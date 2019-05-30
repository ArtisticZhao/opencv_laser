
#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <core/types.hpp>

#include "point_detector.h"
#include "triangle.h"
void test_point(Mat &origin);

using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture(0);
	capture.set(CAP_PROP_FRAME_WIDTH, 5000);
	capture.set(CAP_PROP_FRAME_HEIGHT, 5000);
	int key = 0;
	while (key != 'q')
	{
		Mat frame;
		capture >> frame;
		cout<< frame.size()<<endl;
		test_point(frame);
		imshow("��ȡ��Ƶ", frame);
		key = waitKey(1);	//��ʱ30
	}
	destroyAllWindows();
	return 0;
}

void test_point(Mat &origin)
{
	vector<Point2d> points;
	PointDetector pd;

	//����
	pd.Detect(origin, points);

	//������Ե
	if (points.size() == 1) {
		// measure
		double d;
		//������ѡ����
		cv::circle(origin, points[0], 5, Scalar(0, 255, 0));
		cout << points[0].x << " " << points[0].y << endl;
		//f = measuref(points[0].x, points[0].y, 28, 46, 90);
		d = triangle(points[0].x, points[0].y, 90, 28, 0.0277);
		cout << "d is " << d << endl;
		return;
	}
	else {
		return;
	}
	//for (int i = 0; i < points.size(); ++i) {

	//	//������ѡ����
	//	cv::circle(origin, points[i], 5, Scalar(0, 255, 0));
	//	cout << points[i].x << " " << points[i].y << endl;
	//}
	//return;
	//imshow("origin", origin);

	//waitKey();
}