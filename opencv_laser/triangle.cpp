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

double zxmeasure(double x, double bx, double L, double bi)
{
	double qx, zx;
	qx = bx * pi / 180;
	if (x == 0)
	{
		zx = L * tan(qx);
	}
	else if (bx == 0)
	{
		zx = bi * L / x;
	}
	else
	{
		zx = L / (x / bi + 1 / tan(qx));
	}

	return zx;
}

double zymeasure(double y, double by, double L, double bi)
{
	double qy, zy;
	qy = by / 180 * pi;
	if (y == 0)
	{
		zy = L * tan(qy);
	}
	else if (by == 0)
	{
		zy = bi * L / y;
	}
	else
	{
		zy = L / (y / bi + 1 / tan(qy));
	}
	return zy;
}