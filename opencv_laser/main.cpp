
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
	// ͨ�����������������طֱ���, �趨ֵ�������
	capture.set(CAP_PROP_FRAME_WIDTH, 5000);
	capture.set(CAP_PROP_FRAME_HEIGHT, 5000);
	namedWindow("��ȡ��Ƶ", 1);
	int key = 0;
	setMouseCallback("��ȡ��Ƶ", on_mouse, NULL);
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

	//	//������ѡ����
	//	cv::circle(origin, points[i], 5, Scalar(0, 255, 0));
	//	cout << points[i].x << " " << points[i].y << endl;
	//}
	//return;
	//imshow("origin", origin);

	//waitKey();
}

static void on_mouse(int event, int x, int y, int flags, void* ustc) {
	if (event == EVENT_LBUTTONDOWN)//�����������¼�����  
	{
		//printf( "(%d,%d)", x-width, y-height);//��ӡ��ǰ����ֵ
		cout << "(" << x - width/2 << "," << y - height/2 << ")" << endl;
	}

}