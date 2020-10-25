#pragma once

struct point {
	point() : x(0), y(0), z(0) {};
	point(double x, double y, double z) : x(x), y(y), z(z) {};
	point(const point& p) = default;
	double x;
	double y;
	double z;
};

struct vector {
	vector(point p) : x(p.x), y(p.y), z(p.z) {}
	vector(double x, double y, double z) : x(x), y(y), z(z) {}
	double x;
	double y;
	double z;
};

point operator*(const point& lhs, double rhs) {
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

point operator*=(point& lhs, double rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}

point operator*(double lhs, const point& rhs) {
	return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
}

point operator+(const point& lhs, const point& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

point operator-(const point& lhs, const point& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

point operator+(const point& lhs, const vector& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

void operator+=(point& lhs, const point& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
}

vector operator*(const vector& lhs, double rhs) {
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

vector operator+(const vector& lhs, const vector& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

std::vector<std::vector<point>> operator*=(std::vector<std::vector<point>>& lhs, double rhs) {
	for (auto& v : lhs) {
		for (auto& elem : v) {
			elem *= rhs;
		}
	}
	return lhs;
}