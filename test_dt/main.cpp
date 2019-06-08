#include <iostream>
#include <vector>
#include "vector2.h"
#include "delaunay.h"
using namespace std;
void main() {
	vector<Vector2> points;
	points.push_back(Vector2{ 0, 0 });
	points.push_back(Vector2{ 0, 1 });
	points.push_back(Vector2{ 1, 0 });
	points.push_back(Vector2{ 1, 1 });
	Delaunay triangulation;
	vector<Triangle> triangles = triangulation.triangulate(points);
	cout << triangles.size() << endl;
	if (triangles.size() > 0) {
		
	}
}