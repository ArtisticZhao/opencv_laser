#include "obj_generator.h"
#include "vector2.h"
#include "delaunay.h"
#include "triangle.h"
#include <fstream>


Vector3d OBJ_Model::get_old_point(int index)
{
	return this->points.at(index);
}

void OBJ_Model::add_point(Vector3d new_point, Vector2d new_uv, Vector2i volt)
{
	this->points.push_back(new_point);
	this->uvs.push_back(new_uv);
	this->current_frame_index.push_back(this->uvs.size() - 1);
	this->volts.push_back(volt);
	cout << "[INFO]add new point: " << this->uvs.size() - 1 << endl;
	cout << new_point(0) << " " << new_point(1) << " " << new_point(2) << endl;
}

void OBJ_Model::add_old_point(int index)
{
	this->current_frame_index.push_back(index);
	printf("[INFO]readd point: %d\n", index);
}

void OBJ_Model::create_new_frame()
{
	cout << "new frame index:";
	for (int i = 0; i < this->current_frame_index.size(); i++) {
		cout << " " << this->current_frame_index.at(i);
	}
	cout << endl;
	
	//----------使用 DT 算法对多边形进行分形

	
	if (this->current_frame_index.size() < 3) {
		cout << "[ERROR] 当前平面的点数小于3!" << endl;
		vector<int>().swap(current_frame_index); // 清空当前平面vector
		return;
	}
	// 首先随便估算下该平面的法向量
	Vector3d vnn = this->calc_normal_vector(this->current_frame_index.at(0), this->current_frame_index.at(1), this->current_frame_index.at(2));
	// 找到平面法向量的最大值的位置, 这是投影方向
	int direction = this->find_max_index(vnn);
	int projection_index1 = 0;
	int projection_index2 = 0;
	// 根据投影方向 找到索引值
	switch (direction)
	{
	case 0:
		projection_index1 = 1;
		projection_index2 = 2;
		break;
	case 1:
		projection_index1 = 0;
		projection_index2 = 2;
		break;
	case 2:
		projection_index1 = 0;
		projection_index2 = 1;
		break;
	default:
		break;
	}
	vector<Vector2> dt_points;
	// 根据投影方向 投影三角形
	for (int i = 0; i < this->current_frame_index.size(); i++) {
		dt_points.push_back(Vector2{ this->points.at(current_frame_index.at(i))(projection_index1), this->points.at(current_frame_index.at(i))(projection_index2) });
	}
	Delaunay triangulation;
	vector<Triangle> triangles = triangulation.triangulate(dt_points);
	cout << "find triangles: " << triangles.size() << endl;
	if (triangles.size() > 0) {
		for (int i = 0; i < triangles.size(); i++) {
			int a_index = this->find_point(triangles.at(i).a->x, triangles.at(i).a->y, projection_index1, projection_index2);
			int b_index = this->find_point(triangles.at(i).b->x, triangles.at(i).b->y, projection_index1, projection_index2);
			int c_index = this->find_point(triangles.at(i).c->x, triangles.at(i).c->y, projection_index1, projection_index2);
			// 计算平面法向量
			Vector3d vn = this->calc_normal_vector(a_index, b_index, c_index);
			this->triangle_frame.push_back(OBJ_Triangle{ vn, {a_index, b_index, c_index} });
		}
	}
	vector<int>().swap(current_frame_index); // 清空当前平面vector
}

void OBJ_Model::save_obj()
{
	// save obj
	ofstream outfile;
	outfile.open("data/1.obj");
	// mtllib
	outfile << "mtllib testvt.mtl" << endl;
	// v 顶点数据
	for (auto iter = this->points.begin(); iter != this->points.end(); iter++) {
		outfile << "v " << (*iter)(0) << " "
			<< (*iter)(1) << " "
			<< (*iter)(2) << endl;
	}
	// vt uv纹理数据
	for (auto iter = this->uvs.begin(); iter != this->uvs.end(); iter++) {
		outfile << "vt " << (*iter)(0) << " "
			<< (*iter)(1) << endl;
	}
	// vn 法线
	for (auto iter = this->triangle_frame.begin(); iter != this->triangle_frame.end(); iter++) {
		outfile << "vn " << (*iter).vn(0) << " "
			<< (*iter).vn(1) << " "
			<< (*iter).vn(2) << endl;
	}	
	// use mtl
	outfile << "usemtl xxx" << endl;
	// f
	outfile << "s off" << endl;
	for (int i = 0; i < this->triangle_frame.size(); i++) {
		outfile << "f " << this->triangle_frame.at(i).indexs[0] + 1 << "/" << this->triangle_frame.at(i).indexs[0] + 1 << "/" << i + 1 << " "
			<< this->triangle_frame.at(i).indexs[1] + 1 << "/" << this->triangle_frame.at(i).indexs[1] + 1 << "/" << i + 1 << " "
			<< this->triangle_frame.at(i).indexs[2] + 1 << "/" << this->triangle_frame.at(i).indexs[2] + 1 << "/" << i + 1 << endl;
	}
	// 关闭打开的文件
	outfile.close();
	cout << "[INFO] obj saved" << endl;
}

// 给DT算法之后用, 用于找出对应的顶点号, 只需要在current frame index 中索引的位置查找, 加快效率
int OBJ_Model::find_point(double x, double y, int pi1, int pi2)
{
	int index = 0;
	for (auto iter = this->current_frame_index.begin(); iter != this->current_frame_index.end(); iter++){
		index = (*iter);
		if (this->points.at(index)(pi1) == x && this->points.at(index)(pi2) == y) {
			return index;
		}
	}
	cout << "[ERROR] 未找到的顶点" << endl;
	return 0;
}
// 计算平面法向量
Vector3d OBJ_Model::calc_normal_vector(int a_index, int b_index, int c_index)
{
	return (this->points.at(a_index) - this->points.at(b_index)).cross(this->points.at(c_index) - this->points.at(b_index));
}

int OBJ_Model::find_max_index(Vector3d vnn) {
	double t;
	double max = (t = vnn(0) >= vnn(1) ? vnn(0) : vnn(1)) >= vnn(2) ? t : vnn(2);
	// debug
	cout << "max is " << max << endl << vnn << endl;
	if (max == vnn(0))
		return 0;
	if (max == vnn(1))
		return 1;
	if (max == vnn(2))
		return 2;
}

void OBJ_Model::point_infos()
{
	cout << "----------------------" << endl
		<< "points size:" << this->points.size() << endl
		<< "triangles size" << this->triangle_frame.size() << endl;
	cout << "----------------------" << endl;
}

void OBJ_Model::frame_from_file()
{
	
	ifstream frame_file;
	frame_file.open("data/frame.txt");
	char cmd[100];
	char* p;
	vector<int>().swap(current_frame_index); // 清空当前平面vector
	while (frame_file.getline(cmd, 100)) {
		
		p = cmd;
		while (*p != EOF) {
			this->current_frame_index.push_back(stod(p));
			p = strchr(p, ' ');
			if (p == NULL) {
				break;
			}
			p++;
		}
		this->create_new_frame();
	}
	this->save_obj();
	frame_file.close();
}

void OBJ_Model::save_volts()
{
	ofstream outfile;
	outfile.open("data/volts.txt");
	// vt uv纹理数据
	for (auto iter = this->volts.begin(); iter != this->volts.end(); iter++) {
		outfile<< (*iter)(0) << " "
			<< (*iter)(1) << endl;
	}
	// 关闭打开的文件
	outfile.close();
	cout << "[INFO] volts saved" << endl;
}

void OBJ_Model::delete_point()
{
	this->points.pop_back();
	this->uvs.pop_back();
	this->volts.pop_back();
	this->current_frame_index.pop_back();
	cout << "[INFO]delete a point: " << this->points.size() << endl;
}

