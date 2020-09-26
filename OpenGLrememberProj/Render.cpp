

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <iostream>
#include <vector>
#include <array>
#include <memory>

void InitializeVector(std::vector<std::unique_ptr<double*>>& points, const std::initializer_list<double[3]>& list) {
	for (const auto& point : list) {
		int a[2] = { 1, 2 };
		std::unique_ptr<double*> ptr = std::make_unique<double*>(new double[3] { point[0], point[1], point[2] });
		points.push_back(std::move(ptr));
	}
}

void Render(double delta_time)
{    
	std::vector<std::unique_ptr<double*>> bottom;
	InitializeVector(bottom, { 
		{4, -2, 0},
		{4, 4, 0},
		{0, 0, 0},
		{0, 4, 0},
		{-2, -1, 0},
		{0, 0, 0},
		{3, -6, 0},
		{-2, -1, 0},
		{-5, 2, 0},
		{-3, -4, 0}
	});

	glBegin(GL_TRIANGLE_STRIP); {
		for (const auto& point : bottom) {
			double* tmp = *point;
			glVertex3dv(tmp);
		}
	}
	glEnd();
	/*glBegin(GL_TRIANGLES); {
		double arr[3][3] = {
			{-2, -1, 0},
			{-5, 2, 0},
			{-3, -4, 0},
		};
		glVertex3dv(arr[0]);
		glVertex3dv(arr[1]);
		glVertex3dv(arr[2]);
	}
	glEnd();*/
	

}   

