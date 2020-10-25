#pragma once

#include <vector>
#include <cmath>
#include <limits>

#include "Point.h"

point ermit(point p1, point p2, vector r1, vector r2, double t)
{
	return p1 * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p2 * (-2 * pow(t, 3) + 3 * pow(t, 2)) + r1 * (pow(t, 3) - 2 * pow(t, 2) + t) + r2 * (pow(t, 3) - pow(t, 2));   //посчитанная формула
}

point besie(point p1, point p2, point p3, point p4, double t) {
	return pow(1 - t, 3) * p1 + 3 * t * pow(1 - t, 2) * p2 + 3 * pow(t, 2) * (1 - t) * p3 + pow(t, 3) * p4;
}

unsigned long long factorial(int n) {
	unsigned long long factor = 1;
	for (int i = 1; i <= n; ++i) {
		factor *= i;
	}
	return factor;
}

double get_factor(double u, double n, int index) {
	return (factorial(n) / (factorial(index) * factorial(n - index))) * pow(u, index) * pow(1 - u, n - index);
}

void get_new_besie_surface_point(std::vector<std::vector<point>> points, double u, double v, point& a) {
	point new_p;
	int n = 3, m = 3;
	for (size_t i = 0; i < points.size(); ++i) {
		for (size_t j = 0; j < points[i].size(); ++j) {
			auto fact1 = get_factor(u, n, i);
			auto fact2 = get_factor(v, m, j);
			new_p += points[i][j] * fact1 * fact2;
		}
	}
	a = new_p;
}

double* get_normal(point first, point second, point third, bool reverse = false) {
	vector a = { second.x - first.x, second.y - first.y, second.z - first.z };
	vector b = { third.x - first.x, third.y - first.y, third.z - first.z };
	vector norm = { a.y * b.z - b.y * a.z, -a.x * b.z + b.x * a.z, a.x * b.y - b.x * a.y };
	double len = sqrt(pow(norm.x, 2) + pow(norm.y, 2) + pow(norm.z, 2));
	if (reverse) {
		point tmp = { norm.x / len, norm.y / len, norm.z / len };
		tmp *= -1;
		return (double*)&vector(tmp);
	}
	return (double*)&vector(norm.x / len, norm.y / len, norm.z / len);
}