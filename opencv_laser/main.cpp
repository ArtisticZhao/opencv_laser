#define CAMERA 0
#define ZHENJINGCOM 5
#define DELAY 150

#define CHAFEN

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
#include "point_detector.h"
#include "PlanB.h"
#include "Contest.h"
//void test_point(Mat &origin);

static void on_mouse(int event, int x, int y, int flags, void* ustc);
using namespace cv;
using namespace std;
int width = 0;
int height = 0;
ZhenjingControlor zj_ctrl(ZHENJINGCOM);
LaserCtrlor lz_ctrl(zj_ctrl.get_serial_port());
// obj存储类
OBJ_Model objmodel;
Contest ct;
double point_x, point_y;
double d3[3];
double uv[2];
int flag = 0;  // 存储图片大小标志位
Mat frame_dark;
Mat frame;
VideoCapture capture(CAMERA);
bool is_pid = false;
bool is_pid_y = false;
//void show_up(ifstream& yan_file);
void test_point(Mat& origin, vector<Point2d>& points);
bool measure_lazer();
void process_key(int key);
int main()
{
	Mat jia_picture;
	// 打开剧本
	ifstream yan_file;
	yan_file.open("data/script.txt");
	
	/*PlanB pb;
	pb.set_zj_ctrl(&zj_ctrl);*/
	
	ct.set_zhenjing_ctrl(&zj_ctrl);
	// 通过下面两行设置像素分辨率, 设定值如果超过
	capture.set(CAP_PROP_FRAME_WIDTH, 5000);
	capture.set(CAP_PROP_FRAME_HEIGHT, 5000);
	namedWindow("读取视频", WINDOW_NORMAL);
	int key = 0;
	setMouseCallback("读取视频", on_mouse, NULL);
	bool has_p;
	while (key != 'q')
	{
		has_p = measure_lazer();
		if (is_pid || is_pid_y) {
			zj_ctrl.goal_target(point_x, point_y, is_pid, is_pid_y, has_p);
		}
		key = waitKey(1);
		process_key(key);
	}
	destroyAllWindows();
	return 0;
}

//void show_up(ifstream &yan_file) {
//	char cmd[100];
//	char* p;
//	while (yan_file.getline(cmd, 100)) {
//		if (cmd[0] == 'g' && cmd[1] == ':') {
//			p = strchr(cmd, ':');
//			p++;
//			int x = std::stoi(p);
//			p = strchr(cmd, ',');
//			p++;
//			int y = std::stoi(p);
//			//cout << "go: x " << x << " y " << y << endl;
//			zj_ctrl.goto_volt(x, y);
//			measure_lazer();
//			waitKey(200);
//			/*int key=0;
//			while (key != 'v') {
//				measure_lazer();
//				key = waitKey(5);
//			}*/
//			// add point to backup
//			xyz_back.push_back(d3[0]);
//			xyz_back.push_back(d3[1]);
//			xyz_back.push_back(d3[2]);
//			uvs_back.push_back(uv[0]);
//			uvs_back.push_back(uv[1]);
//			printf("[INFO]count %d point x:%f, y:%f, z:%f  u:%f, v:%f\n", uvs_back.size() / 2, d3[0], d3[1], d3[2], uv[0], uv[1]);
//		}
//		else if (cmd[0] == 'c' && cmd[1] == ':') {
//			// 添加重复的点到模型
//			p = strchr(cmd, ':');
//			p++;
//			int x = std::stoi(p);
//			cout << "add " << x << endl;
//			// add point
//			readd_index = x - 1;
//			xyz_points.push_back(xyz_back.at(readd_index * 3));
//			xyz_points.push_back(xyz_back.at(readd_index * 3 + 1));
//			xyz_points.push_back(xyz_back.at(readd_index * 3 + 2));
//
//			uvs.push_back(uvs_back.at(readd_index * 2));
//			uvs.push_back(uvs_back.at(readd_index * 2 + 1));
//			printf("[INFO]point x:%f, y:%f, z:%f  u:%f, v:%f\n", xyz_points.at(readd_index * 3), xyz_points.at(readd_index * 3 + 1), xyz_points.at(readd_index * 3 + 2), uvs.at(readd_index * 2), uvs.at(readd_index * 2 + 1));
//		}
//		else if (cmd[0] == 'm') {
//			// new frame
//			cout << "points from " << xyz_index << " to " << xyz_points.size() / 3 << endl;
//			vector<Vector2> dt_points;
//			for (int i = xyz_index; i < xyz_points.size(); i += 3) {
//				int i2 = i + 2;
//				dt_points.push_back(Vector2{ xyz_points.at(i), xyz_points.at(i2) });// only use x,y to dt
//			}
//			Delaunay triangulation;
//			vector<Triangle> triangles = triangulation.triangulate(dt_points);
//			cout << "find triangles: " << triangles.size() << endl;
//			if (triangles.size() > 0) {
//				for (int i = 0; i < triangles.size(); i++) {
//					double find_x = triangles.at(i).a->x;
//					int a_index = find_index(xyz_index, xyz_points, find_x);
//					// a 3d point is xyz_points.at(x_index) +1 +2
//					find_x = triangles.at(i).b->x;
//					int b_index = find_index(xyz_index, xyz_points, find_x);
//					find_x = triangles.at(i).c->x;
//					int c_index = find_index(xyz_index, xyz_points, find_x);
//					// 计算向量
//					vector3 v3 = cross_product(vector3{
//						(xyz_points.at(a_index) - xyz_points.at(b_index)),
//						(xyz_points.at(a_index + 1) - xyz_points.at(b_index + 1)),
//						(xyz_points.at(a_index + 2) - xyz_points.at(b_index + 2)) },
//						vector3{
//						(xyz_points.at(c_index) - xyz_points.at(b_index)),
//					(xyz_points.at(c_index + 1) - xyz_points.at(b_index + 1)),
//					(xyz_points.at(c_index + 2) - xyz_points.at(b_index + 2))
//						});
//					// 保存
//					frames.push_back(obj_frame{ {v3.x, v3.y, v3.z},{a_index, b_index, c_index} });
//				}
//				xyz_index = xyz_points.size();
//			}
//		}
//		else if (cmd[0] == 'h') {
//			waitKey();
//		}
//		
//	}
//}

void test_point(Mat &origin, vector<Point2d>& points)
{
	//vector<Point2d> points;
	PointDetector pd;
	//检测点
	pd.Detect(origin, points);
	return;
	////遍历边缘
	//if (points.size() == 1) {
	//	//画出所选区域
	//	cv::circle(origin, points[0], 5, Scalar(0, 255, 0));
	//	cout << points[0].x- origin.size().width / 2 << " " << points[0].y - origin.size().height/2 << endl;
	//	return;
	//}
	//else {
	//	return;
	//}
	//for (int i = 0; i < points.size(); ++i) {

	//	//画出所选区域
	//	cv::circle(origin, points[i], 5, Scalar(0, 255, 0));
	//	cout << points[i].x << " " << points[i].y << endl;
	//}
	//return;
	//imshow("origin", origin);

}

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

bool measure_lazer() {

	bool has_point = false;
	vector<Point2d> points;
#ifdef CHAFEN

	// light up laser

	//Sleep(DELAY);
	capture >> frame;
	// turn off laser
	lz_ctrl.laser_off();
	Sleep(DELAY);
	capture >> frame_dark;
	lz_ctrl.laser_on();
	if (flag == 0) {
		flag = 1;
		width = frame.size().width;
		height = frame.size().height;
		cout << "width is " << width << " height is " << height << endl;
	}
	get_point(frame, frame_dark, points);
#else
	capture >> frame;
	test_point(frame, points);
	if (flag == 0) {
		flag = 1;
		width = frame.size().width;
		height = frame.size().height;
		cout << "width is " << width << " height is " << height << endl;
	}
#endif
	//遍历边缘
	if (points.size() != 0) {
		has_point = true;
		// 选择轮廓范围: 选择最大值
		// measure			
		//画出所选区域
		cv::circle(frame, points[0], 5, Scalar(0, 255, 0), 5);
		//cout << points[0].x << " " << points[0].y << endl;
		//cout << points[0].x - width / 2 << " " << points[0].y - height / 2 << endl;
		// calc xyz
		//zj_ctrl.goal_target(points[0].x, points[0].y);
		zmeasure(points[0].x - width / 2, (points[0].y - height / 2), zj_ctrl.get_angle_x(), zj_ctrl.get_angle_y(), 21, 1139.175, d3, 3);
		uv[0] = points[0].x / width;
		uv[1] = points[0].y / height;
		point_x = points[0].x;
		point_y = points[0].y;
		//printf("x:%f, y:%f, z:%f\n", d3[0], d3[1], d3[2]);
	}
	//test_point(frame);
	// draw cross
	line(frame, Point2d(0, height / 2), Point2d(width, height / 2), Scalar(0, 255, 255), 1, LINE_AA);
	line(frame, Point2d(width / 2, 0), Point2d(width / 2, height), Scalar(0, 255, 255), 1, LINE_AA);
	imshow("读取视频", frame);
	return has_point;
}

void process_key(int key) {
	if (key != -1) {
		zj_ctrl.zhenjing_control(key); // 1 2 3 4 5 w a s d
		lz_ctrl.setduty(key);  // q e 改变大小
		if (key == 'y') {
			ct.show_all();
		}
		// 拍照片
		else if (key == 'p') {
			// take photo
			Mat flip_frame;
			flip(frame_dark, flip_frame, 0);
			imwrite("data/1.png", flip_frame);
			std::cout << "[INFO] image saved!" << std::endl;
		}
		// 添加新点
		else if (key == 'v') {
			Vector3d point(d3[0], d3[1], d3[2]);
			Vector2d v_uv(uv[0], uv[1]);
			Vector2i volts(zj_ctrl.get_x(), zj_ctrl.get_y());
			objmodel.add_point(point, v_uv, volts);
		}
		// 显示信息
		else if (key == 'i') {
			// show info
			zj_ctrl.show_volts();
			objmodel.point_infos();
		}
		
		// 添加旧点
		else if (key == 'c') {
			// re add
			int index = 0;
			cout << "输入index:";
			cin >> index;
			objmodel.add_old_point(index);
		}
		else if (key == 'x')
		{
			objmodel.delete_point();
		}
		// 将已添加的点创建一个新的平面
		else if (key == 'm') {
			objmodel.create_new_frame();
		}
		// 保存obj模型
		else if (key == 'o') {
			objmodel.save_obj();
		}
		// 根据文档转动振镜
		else if (key == 'g') {
			// read cmd
			ifstream infile;
			infile.open("data/cmd.txt");
			char cmd[100];
			char* p;
			infile.getline(cmd, 100);
			if (cmd[0] == 'g' && cmd[1] == ':') {
				p = strchr(cmd, ':');
				p++;
				int x = std::stoi(p);
				p = strchr(cmd, ',');
				p++;
				int y = std::stoi(p);
				cout << "go: x " << x << " y " << y << endl;
				zj_ctrl.goto_volt(x, y);
			}
		}
		
		else if (key == 'h')
		{
			is_pid = true;
			is_pid_y = true;
		}
		else if (key == 'H')
		{
			is_pid = false;
			is_pid_y = false;
		}
		else if (key == 'u') {
			objmodel.frame_from_file();
		}
		// save volts
		else if (key == 'l') {
			objmodel.save_volts();
		}
		else if (key == 'j') {
			ct.adjust();
		}
}
}