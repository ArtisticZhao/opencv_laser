
#include <iostream>  
#include <fstream>

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <core/types.hpp>
#include "pid.h"
#include "LaserCtrlor.h"
#include "zhenjing_control.h"
#include "my_point_detector.h"
#include "triangle_cxf.h"
#include "vector2.h"
#include "delaunay.h"
#include "obj_generator.h"
//void test_point(Mat &origin);
int find_index(int start, vector<double> points, double val);
vector3 cross_product(vector3 a, vector3 b);
static void on_mouse(int event, int x, int y, int flags, void* ustc);
using namespace cv;
using namespace std;
int width = 0;
int height = 0;
ZhenjingControlor zj_ctrl(5);
int main()
{
	LaserCtrlor lz_ctrl(7);
	VideoCapture capture(2);
	vector<double> xyz_points;
	vector<double> uvs;
	vector<obj_frame> frames;
	int xyz_index=0;
	// 通过下面两行设置像素分辨率, 设定值如果超过
	capture.set(CAP_PROP_FRAME_WIDTH, 5000);
	capture.set(CAP_PROP_FRAME_HEIGHT, 5000);
	namedWindow("读取视频", WINDOW_NORMAL);
	int key = 0;
	setMouseCallback("读取视频", on_mouse, NULL);
	int flag = 0;
	while (key != 'q')
	{
		Mat frame;
		Mat frame_dark;
		vector<Point2d> points;
		double d3[3];
		double uv[2];
		// light up laser
		lz_ctrl.laser_on();
		Sleep(250);
		capture >> frame;
		//Sleep(50);
		// turn off laser
		lz_ctrl.laser_off();
		Sleep(250);
		capture >> frame_dark;
		//Sleep(50);
		if (flag == 0) {
			flag = 1;
			width = frame.size().width;
			height = frame.size().height;
			cout << "width is " << width << " height is " << height << endl;
		}
		get_point(frame, frame_dark, points);
		//遍历边缘
		if (points.size() == 1) {
			// measure			
			//画出所选区域
			cv::circle(frame, points[0], 5, Scalar(0, 255, 0));
			//cout << points[0].x << " " << points[0].y << endl;
			cout << points[0].x - frame.size().width / 2 << " " << points[0].y - frame.size().height / 2 << endl;
			// calc xyz
			//zj_ctrl.goal_target(points[0].x, points[0].y);
			zmeasure(points[0].x - frame.size().width / 2, (points[0].y - frame.size().height / 2), zj_ctrl.get_angle_x(), zj_ctrl.get_angle_y(), 18.4, 1090, d3, 3);
			uv[0] = points[0].x / width;
			uv[1] = 1 - points[0].y / height;
			printf("x:%f, y:%f, z:%f\n", d3[0], d3[1], d3[2]);
		}
		//test_point(frame);
		// draw cross
		line(frame, Point2d(0, height / 2), Point2d(width, height / 2), Scalar(0, 255, 255), 1, LINE_AA);
		line(frame, Point2d(width / 2, 0), Point2d(width / 2, height), Scalar(0, 255, 255), 1, LINE_AA);
		imshow("读取视频", frame);
		key = waitKey(1);	//延时30
		if (key != -1) {
			zj_ctrl.zhenjing_control(key);
			if (key == 'v') {
				// add point
				xyz_points.push_back(d3[0]);
				xyz_points.push_back(d3[1]);
				xyz_points.push_back(d3[2]);

				uvs.push_back(uv[0]);
				uvs.push_back(uv[1]);
				printf("[INFO]point x:%f, y:%f, z:%f  u:%f, v:%f\n", d3[0], d3[1], d3[2], uv[0], uv[1]);
			}
			else if (key == 'p') {
				// take photo
				imwrite("1.bmp", frame_dark);
				cout << "[INFO] image saved!" << endl;
			}
			else if (key == 'm') {
				// new frame
				cout << "points from " << xyz_index << " to " << xyz_points.size() / 3 << endl;
				vector<Vector2> dt_points;
				for (int i = xyz_index; i < xyz_points.size(); i += 3) {
					int i2 = i + 1;
					dt_points.push_back(Vector2{ xyz_points.at(i), xyz_points.at(i2) });// only use x,y to dt
				}
				Delaunay triangulation;
				vector<Triangle> triangles = triangulation.triangulate(dt_points);
				cout <<"find triangles: "<< triangles.size() << endl;
				if (triangles.size() > 0) {
					for (int i = 0; i < triangles.size(); i++) {
						double find_x = triangles.at(i).a->x;
						int a_index = find_index(xyz_index, xyz_points, find_x);
						// a 3d point is xyz_points.at(x_index) +1 +2
						find_x = triangles.at(i).b->x;
						int b_index = find_index(xyz_index, xyz_points, find_x);
						find_x = triangles.at(i).c->x;
						int c_index = find_index(xyz_index, xyz_points, find_x);
						// 计算向量
						vector3 v3 = cross_product(vector3{
							(xyz_points.at(a_index) - xyz_points.at(b_index)),
							(xyz_points.at(a_index + 1) - xyz_points.at(b_index + 1)),
							(xyz_points.at(a_index + 2) - xyz_points.at(b_index + 2)) },
							vector3{
							(xyz_points.at(c_index) - xyz_points.at(b_index)),
						(xyz_points.at(c_index + 1) - xyz_points.at(b_index + 1)),
						(xyz_points.at(c_index + 2) - xyz_points.at(b_index + 2))
							});
						// 保存
						frames.push_back(obj_frame{ {v3.x, v3.y, v3.z},{a_index, b_index, c_index} });
					}
					xyz_index = xyz_points.size();
				}
			}
			else if (key == 'o') {
				// save obj
				ofstream outfile;
				outfile.open("1.obj");
				int temp1, temp2;
				// v
				for (int i = 0; i < xyz_points.size(); i+=3) {
					temp1 = i + 1;
					temp2 = i + 2;
					outfile << "v " << xyz_points.at(i) << " "
						<< xyz_points.at(temp1) << " "
						<< xyz_points.at(temp2) << endl;
				}
				//vt
				for (int i = 0; i < uvs.size(); i += 2) {
					temp1 = i + 1;
					outfile << "vt " << uvs.at(i) << " "
						<< uvs.at(temp1) << endl;
				}
				// vn
				for (int i = 0; i < frames.size(); i++) {
					outfile << "vn " << frames.at(i).vn[0] << " "
						<< frames.at(i).vn[1] << " "
						<< frames.at(i).vn[2] << endl;
				}
				// f
				outfile << "s off" << endl;
				for (int i = 0; i < frames.size(); i++) {
					outfile << "f " << frames.at(i).index3[0] / 3 + 1 << "/" << frames.at(i).index3[0] / 3 + 1 << "/" << i + 1 << " "
						<< frames.at(i).index3[1] / 3 + 1 << "/" << frames.at(i).index3[1] / 3 + 1 << "/" << i + 1 << " "
						<< frames.at(i).index3[2] / 3 + 1 << "/" << frames.at(i).index3[2] / 3 + 1 << "/" << i + 1 << endl;
				}

				// 关闭打开的文件
				outfile.close();
				cout << "[INFO] obj saved" << endl;
			}
		}
	}
	destroyAllWindows();
	return 0;
}
vector3 cross_product(vector3 a, vector3 b) {
	vector3 c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	double mod = sqrt((c.x) * (c.x) + (c.y) * (c.y) + (c.z) * (c.z));
	c.x = c.x / mod;
	c.y = c.y / mod;
	c.z = c.z / mod;
	return c;
}
int find_index(int start,vector<double> points, double val) {
	int x_index = 0;
	for (x_index = start; x_index < points.size(); x_index += 3) {
		if (points.at(x_index) == val) {
			break;
		}
	}
	return x_index;
}
//
//void test_point(Mat &origin)
//{
//	vector<Point2d> points;
//	PointDetector pd;
//	//检测点
//	pd.Detect(origin, points);
//
//	//遍历边缘
//	if (points.size() == 1) {
//		// measure
//		double d;
//		//画出所选区域
//		cv::circle(origin, points[0], 5, Scalar(0, 255, 0));
//		cout << points[0].x- origin.size().width / 2 << " " << points[0].y - origin.size().height/2 << endl;
//		//f = measuref(points[0].x, points[0].y, 28, 46, 90);
//		d = triangle(points[0].x, points[0].y, 90, 28, 0.0277);
//		cout << "d is " << d << endl;
//		return;
//	}
//	else {
//		return;
//	}
//	//for (int i = 0; i < points.size(); ++i) {
//
//	//	//画出所选区域
//	//	cv::circle(origin, points[i], 5, Scalar(0, 255, 0));
//	//	cout << points[i].x << " " << points[i].y << endl;
//	//}
//	//return;
//	//imshow("origin", origin);
//
//	//waitKey();
//}
//
static void on_mouse(int event, int x, int y, int flags, void* ustc) {
	if (event == EVENT_LBUTTONDOWN)//鼠标左键按下事件发生  
	{
		//printf( "(%d,%d)", x-width, y-height);//打印当前坐标值
		zj_ctrl.set_target(x, y);
		cout << "(" << x - width/2 << "," << y - height/2 << ")" << endl;
	}
	else if(event == EVENT_RBUTTONDOWN){  // 右键点击
		// TODO  auto move laser
		if (x<width / 3 && y>height / 3 && y < height / 3 * 2) {
			// left
			zj_ctrl.zhenjing_control('a');
		}
		else if (x > 2 * width / 3 && y > height / 3 && y < height / 3 * 2) {
			// right
			zj_ctrl.zhenjing_control('d');
		}
		else if (x > width / 3 && x < width * 2 / 3 && y < height / 3) {
			// up
			zj_ctrl.zhenjing_control('w');
		}
		else if (x > width / 3 && x < width * 2 / 3 && y > 2 * height / 3) {
			// down
			zj_ctrl.zhenjing_control('s');
		}
	}

}