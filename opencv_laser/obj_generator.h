#pragma once
#include <vector>
#include <iostream>  

#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
struct obj_frame
{
	double vn[3];
	int index3[3];
};

struct vector3 {
	double x;
	double y;
	double z;
};

struct OBJ_Triangle {
	Vector3d vn;	// 三角面法向量
	int indexs[3]; // 三角面索引值
};

class OBJ_Model {
private:
	vector<Vector3d> points;
	vector<Vector2d> uvs;
	vector<int> current_frame_index;  // 当前平面索引
	vector<OBJ_Triangle> triangle_frame;  //三角化平面
public:
	void add_point(Vector3d new_point, Vector2d new_uv);
	void add_old_point(int index);
	void create_new_frame();
	void save_obj();
	int find_point(double x, double y, int pi1, int pi2);
	Vector3d calc_normal_vector(int a_index, int b_index, int c_index);
	int find_max_index(Vector3d vnn);
	void point_infos();
};