#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <algorithm>
#include <cmath>

struct point {
	point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	point(double x, double y) {
		this->x = x;
		this->y = y;
		this->z = 0;
	}
	double x;
	double y;
	double z;
};

size_t size = 0;
size_t convex_circle_count_points = 0;


void append_convex_circle(std::vector<point>& panel) {
	convex_circle_count_points = 0;
	point first_point = { 4, -2 }; //point with coor 4, -2
	point second_point = { 4, 4 }; //point with coor 4, 4
	point center = { (first_point.x + second_point.x) / 2, (first_point.y + second_point.y) / 2 };
	double radius = sqrt(pow(first_point.x - second_point.x, 2) + pow(first_point.y - second_point.y, 2)) / 2;


	panel.emplace_back(center.x, center.y); //center point
	panel.emplace_back(first_point.x, first_point.y); //first_point
	convex_circle_count_points += 2;
	for (double i = -90; i < 90; ++i) {
		double iter = i * M_PI / 180;
		panel.emplace_back(radius * cos(iter) + center.x, radius * sin(iter) + center.y);
		++convex_circle_count_points;
	}
	panel.emplace_back(second_point.x, second_point.y); //last_point
	++convex_circle_count_points;
}

void append_nested_circle(std::vector<point>& panel) {
	point first_point = { -3, -4 };//point with coor -3, -4
	point second_point = { 3, -6 };//point with coor 3, -6
	point third_point = { 0, -4 }; // point M
	double A = second_point.x - first_point.x;
	double B = second_point.y - first_point.y;
	double C = third_point.x - first_point.x;
	double D = third_point.y - first_point.y;
	double E = A * (first_point.x + second_point.x) + B * (first_point.y + second_point.y);
	double F = C * (first_point.x + third_point.x) + D * (first_point.y + third_point.y);
	double G = 2 * (A * (third_point.y - second_point.y) - B * (third_point.x - second_point.x));
	point c = { (D * E - B * F) / G, (A * F - C * E) / G };
	double R = sqrt(pow(first_point.x - c.x, 2) + pow(first_point.y - c.y, 2));

	panel.emplace_back(-2 , -1); //point with coor -2, -1
	panel.emplace_back(-3, -4);//point with coor -3, -4
	for (int i = 105; i > 37; --i) {
		double iter = i * M_PI / 180;
		panel.emplace_back(R * cos(iter) + c.x, R * sin(iter) + c.y);
	}
	panel.emplace_back(3, -6);//point with coor 3, -6
}

const double* get_color(const double(&colors)[2][3], bool& color_flag) {
	if (color_flag) {
		color_flag = false;
		return colors[0];
	}
	else {
		color_flag = true;
		return colors[1];
	}
}
 
void rotate_top_panel(std::vector<point>& panel, double degrees) {
	double radians = degrees * M_PI / 180;
	for (auto& point : panel) {
		double x = point.x, y = point.y;
		double new_x = x * cos(radians) - y * sin(radians);
		double new_y = x * sin(radians) + y * cos(radians);
		point.x = new_x;
		point.y = new_y;
	}
}

std::vector<point> filling_vectors(std::vector<point>& bottom) {
	
	append_convex_circle(bottom);
	append_nested_circle(bottom);
	auto top = bottom;
	for (auto& point : top) {
		point.z = 1;
	}
	rotate_top_panel(top, 20);
	return top;
}

void draw_extreme_panels(const std::vector<point>& bottom, const std::vector<point>& top, const double* color) {
	glColor3dv(color);
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			glVertex3dv((double*)&bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			glVertex3dv((double*)&top[i]);
		}
	}
	glEnd();
	double tmp[3] = { 0, 1, 0 };
	glColor3dv(tmp);
	size_t last_convex_index = size + convex_circle_count_points;
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glVertex3dv((double*)&bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glVertex3dv((double*)&top[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < bottom.size(); ++i) {
			glVertex3dv((double*)&bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < top.size(); ++i) {
			glVertex3dv((double*)&top[i]);
		}
	}
	glEnd();
}

void draw_side_panels(const std::vector<point>& bottom, const std::vector<point>& top, const double(&colors)[2][3]) {
	std::vector<int> indexes = { 6, 5, 0, 1, 3, 4, 8, 9 };
	size_t last_convex_index = size + convex_circle_count_points;
	bool color_flag = false;
	glBegin(GL_QUAD_STRIP); {
		for (auto i : indexes) {
			if (i != 1) {
				glColor3dv(get_color(colors, color_flag));
				glVertex3dv((double*)&bottom[i]);
				glVertex3dv((double*)&top[i]);
			}
			else {
				for (size_t j = size; j < last_convex_index; ++j) {
					glColor3dv(get_color(colors, color_flag));
					glVertex3dv((double*)&bottom[j]);
					glVertex3dv((double*)&top[j]);
				}
			}
		}
		for (size_t j = last_convex_index; j < bottom.size(); ++j) {
			glColor3dv(get_color(colors, color_flag));
			glVertex3dv((double*)&bottom[j]);
			glVertex3dv((double*)&top[j]);
		}
	}
	glEnd();
}

void Render(double delta_time) {
	std::vector<point> bottom = {
		{4, -2, 0},//0
		{4, 4, 0},//1
		{0, 0, 0},//2
		{0, 4, 0},//3
		{-2, -1, 0},//4
		{0, 0, 0},//5
		{3, -6, 0},//6
		{-2, -1, 0},//7
		{-5, 2, 0},//8
		{-3, -4, 0}//9
	};
	size = bottom.size();
	auto top = filling_vectors(bottom);
	double extreme_color[] = { 0.2, 0.3, 0.7 };
	draw_extreme_panels(bottom, top, extreme_color);
	const double side_colors[][3] = { 
		{0.7, 0.2, 0.4},
		{0.1, 0.8, 0.3} 
	};
	draw_side_panels(bottom, top, side_colors);
}