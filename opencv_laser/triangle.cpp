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