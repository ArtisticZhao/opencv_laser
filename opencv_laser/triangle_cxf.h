#pragma once
#ifndef TRIANGLE_CXF_H
#include <Eigen/Dense>
using namespace Eigen;

double triangle(double x, double y, double ar, double c, double f);
double measuref(double x, double y, double c, double L, double ar);
void zmeasure(double x, double y, double bx, double by, double L, double bi, double* a, int n, double angle);

Vector3d find(Vector3d before, double angle);
#endif // !TRIANGLE_CXF_H
