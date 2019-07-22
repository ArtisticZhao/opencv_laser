#include "obj_generator.h"
#include "vector2.h"
#include "delaunay.h"
#include <fstream>


void OBJ_Model::add_point(Vector3d new_point, Vector2d new_uv)
{
	this->points.push_back(new_point);
	this->uvs.push_back(new_uv);
	this->current_frame_index.push_back(this->uvs.size() - 1);
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
	
	//使用 DT 算法对多边形进行分形
	vector<Vector2> dt_points;
	for (int i = 0; i < this->current_frame_index.size(); i++) {
		dt_points.push_back(Vector2{ this->points.at(current_frame_index.at(i))(0), this->points.at(current_frame_index.at(i))(2) });// only use x,y to dt
	}
	Delaunay triangulation;
	vector<Triangle> triangles = triangulation.triangulate(dt_points);
	cout << "find triangles: " << triangles.size() << endl;
	if (triangles.size() > 0) {
		for (int i = 0; i < triangles.size(); i++) {
			int a_index = this->find_point(triangles.at(i).a->x);
			int b_index = this->find_point(triangles.at(i).b->x);
			int c_index = this->find_point(triangles.at(i).c->x);
			// 计算平面法向量
			Vector3d vn = (this->points.at(a_index) - this->points.at(b_index)).cross(this->points.at(c_index) - this->points.at(b_index));
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

// 给DT算法之后用, 用于找出对应的顶点号, 使用x坐标
int OBJ_Model::find_point(double x)
{
	int index = 0;
	for (index = 0; index < this->points.size(); index++) {
		if (this->points.at(index)(0) == x) {
			break;
		}
	}
	return index;
}

void OBJ_Model::point_infos()
{
	cout << "----------------------" << endl
		<< "points size:" << this->points.size() << endl
		<< "triangles size" << this->triangle_frame.size() << endl;
	cout << "----------------------" << endl;
}
