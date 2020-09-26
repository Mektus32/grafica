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

const size_t size(10);
size_t convex_circle_count_points = 0;

void initialize_vectors(std::vector<std::unique_ptr<double*>>& points, const std::initializer_list<double[3]>& list) {
	for (const auto& point : list) {
		int a[2] = { 1, 2 };
		std::unique_ptr<double*> ptr = std::make_unique<double*>(new double[3] { point[0], point[1], point[2] });
		points.push_back(std::move(ptr));
	}
}

void append_convex_circle(std::vector<std::unique_ptr<double*>>& panel) {
	convex_circle_count_points = 0;
	double first_point[] = { (*panel[0])[0], (*panel[0])[1] }; //point with coor 4, -2
	double second_point[] = { (*panel[1])[0], (*panel[1])[1] }; //point with coor 4, 4
	double center[] = { (first_point[0] + second_point[0]) / 2, (first_point[1] + second_point[1]) / 2 };
	double radius = sqrt(pow(first_point[0] - second_point[0], 2) + pow(first_point[1] - second_point[1], 2)) / 2;


	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ center[0], center[1], (double)0 }))); //center point
	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ first_point[0], first_point[1], (double)0 }))); //first_point
	convex_circle_count_points += 2;
	for (double i = -90; i < 90; ++i) {
		double iter = i * M_PI / 180;
		panel.push_back(std::move(std::make_unique<double*>(new double[3]{ radius * cos(iter) + center[0], radius * sin(iter) + center[1], (double)0 })));
		++convex_circle_count_points;
	}
	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ second_point[0], second_point[1], (double)0 }))); //last_point
	++convex_circle_count_points;
}

void append_nested_circle(std::vector<std::unique_ptr<double*>>& panel) {
	double first_point[] = { (*panel[9])[0], (*panel[9])[1] };
	double second_point[] = { (*panel[6])[0], (*panel[6])[1] };
	double third_point[] = { 0, -4 }; // point M
	double A = second_point[0] - first_point[0];
	double B = second_point[1] - first_point[1];
	double C = third_point[0] - first_point[0];
	double D = third_point[1] - first_point[1];
	double E = A * (first_point[0] + second_point[0]) + B * (first_point[1] + second_point[1]);
	double F = C * (first_point[0] + third_point[0]) + D * (first_point[1] + third_point[1]);
	double G = 2 * (A * (third_point[1] - second_point[1]) - B * (third_point[0] - second_point[0]));
	double c[] = { (D * E - B * F) / G, (A * F - C * E) / G };
	double R = sqrt(pow(first_point[0] - c[0], 2) + pow(first_point[1] - c[1], 2));
	
	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ (*panel[4])[0], (*panel[4])[1], 0 })));
	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ (*panel[9])[0], (*panel[9])[1], 0 })));
	for (int i = 105; i > 37; --i) {
		double iter = i * M_PI / 180;
		panel.push_back(std::move(std::make_unique<double*>(new double[3]{ R * cos(iter) + c[0], R * sin(iter) + c[1], 0})));
	}
	panel.push_back(std::move(std::make_unique<double*>(new double[3]{ (*panel[6])[0], (*panel[6])[1], 0 })));
}

void filling_vectors(std::vector<std::unique_ptr<double*>>& bottom, std::vector<std::unique_ptr<double*>>& top) {
	initialize_vectors(bottom, {
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
	});
	append_convex_circle(bottom);
	append_nested_circle(bottom);
	for (const auto& point : bottom) {
		top.push_back(std::move(std::make_unique<double*>(new double[3]{ (*point)[0], (*point)[1], 1 })));
	}
}

void draw_extreme_panels(const std::vector<std::unique_ptr<double*>>& bottom, const std::vector<std::unique_ptr<double*>>& top, const double* color) {
	
	glColor3dv(color);
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			glVertex3dv(*bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			glVertex3dv(*top[i]);
		}
	}
	glEnd();
	double tmp[3] = { 0, 1, 0 };
	glColor3dv(tmp);
	size_t last_convex_index = size + convex_circle_count_points;
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glVertex3dv(*bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glVertex3dv(*top[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < bottom.size(); ++i) {
			glVertex3dv(*bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < top.size(); ++i) {
			glVertex3dv(*top[i]);
		}
	}
	glEnd();
}

void draw_side_panels(const std::vector<std::unique_ptr<double*>>& bottom, const std::vector<std::unique_ptr<double*>>& top, const double* color) {
	std::vector<int> indexes = { 6, 5, 0, 1, 3, 4, 8, 9 };
	size_t last_convex_index = size + convex_circle_count_points;
	glColor3dv(color);
	glBegin(GL_QUAD_STRIP); {
		for (auto i : indexes) {
			if (i != 1) {
				glVertex3dv(*bottom[i]);
				glVertex3dv(*top[i]);
			}
			else {
				for (size_t j = size; j < last_convex_index; ++j) {
					glVertex3dv(*bottom[j]);
					glVertex3dv(*top[j]);
				}
			}
		}
		for (size_t j = last_convex_index; j < bottom.size(); ++j) {
			glVertex3dv(*bottom[j]);
			glVertex3dv(*top[j]);
		}
	}
	glEnd();
}

void Render(double delta_time) {
	std::vector<std::unique_ptr<double*>> bottom, top;
	filling_vectors(bottom, top);
	double extreme_color[] = { 0.2, 0.3, 0.7 };
	draw_extreme_panels(bottom, top, extreme_color);
	double side_color[] = { 0.7, 0.2, 0.4 };
	draw_side_panels(bottom, top, side_color);
}   

