#include "triangle.h"
#include <math.h>

const double pi(3.1415926535);

double triangle(double x, double y, double ar, double c, double f)
{
	double s1, s2, s, q, L;
	q = ar * pi / 180;
	s1 = f / tan(q);
	s2 = sqrt(x * x + y * y);
	s = s1 + s2 * 44e-6;
	L = c * f / s;
	return L;

}

double measuref(double x, double y,double c, double L, double ar) {
	double c1, c2, s2, s,f, q;
	q = ar * pi / 180;
	c1 = L / tan(q);
	c2 = c - c1;
	s = sqrt(x * x + y * y);
	s2 = s * 44.0e-6;
	f = L * s2 / c2;
	return f;
}



//double *zmeasure(double x,double y,double bx,double by,double L,double bi,double z[3])
void zmeasure(double x, double y, double bx, double by, double L, double bi, double* a, int n)
{
	double tanbeta, qx, qy, l, tanafa, zsum, zx, zy, zz;
	double z[3];
	qx = bx * pi / 180;
	qy = by * pi / 180;
	l = sqrt(x * x + y * y);
	tanbeta = 1 / (cos(qy) + tan(qx));
	tanafa = bi / l;
	zsum = L / (1 / tanbeta + 1 / tanafa);
	zx = zsum / tanafa;
	zy = zsum * cos(by);
	zz = zsum * sin(by);
	z[0] = zx;
	z[1] = zy;
	z[2] = zz;

	for (int i = 0; i < n; i++)
		a[i] = z[i];
}