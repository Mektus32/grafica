

#include "Render.h"

#include <cmath>
#define _USE_MATH_DEFINES

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>


struct point {
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

vector operator*(const vector& lhs, double rhs) {
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

vector operator+(const vector& lhs, const vector& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

point ermit(point p1, point p2, vector r1, vector r2, double t)
{
	return p1 * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + p2 * (-2 * pow(t, 3) + 3 * pow(t, 2)) + r1 * (pow(t, 3) - 2 * pow(t, 2) + t) + r2 * (pow(t, 3) - pow(t, 2));   //посчитанная формула
}

point besie(point p1, point p2, point p3, point p4, double t) {
	return pow(1 - t, 3) * p1 + 3 * t * pow(1 - t, 2) * p2 + 3 * pow(t, 2) * (1 - t) * p3 + pow(t, 3) * p4;
}

double t_max = 0;


void Render(double delta_time)
{    
	t_max += delta_time / 5; //t_max становится = 1 за 5 секунд
	if (t_max > 1) t_max = 0; //после обнуляется
	point P1 = { 0,0,0 }; //Наши точки
	point P2 = { -4,6,7 };
	point P3 = { 10,10,0 };
	point P4 = { 4, 8, 3 };
	vector r1 = { P2 - P1 };
	vector r2 = { P4 - P3 };
	glColor3d(0, 1, 0);
	glLineWidth(4);
	glBegin(GL_LINE_STRIP);
	/*glVertex3d(P2.x, P2.y, P2.z);
	glVertex3d(P1.x, P1.y, P1.z);*/
	for (double t = 0; t <= 1.0001; t += 0.01)
	{	
		
		auto p = ermit(P1, P4, r1, r2, t);
		//auto p = besie(P1, P2, P3, P4, t);
		glVertex3d(p.x, p.y, p.z); //Рисуем точку P
	}
	/*glVertex3d(P3.x, P3.y, P3.z);
	glVertex3d(P4.x, P4.y, P4.z);*/
	glEnd();
	//glColor3d(1, 0, 1);
	//glLineWidth(1); //возвращаем ширину линии = 1
	////нарисуем все точки
	//glPointSize(10);
	//glBegin(GL_POINTS);
	//glVertex4dv(P);
	//glEnd();
	glPointSize(10);
	glColor3d(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex3d(P1.x, P1.y, P1.z);
	glVertex3d(P2.x, P2.y, P2.z);
	glVertex3d(P3.x, P3.y, P3.z);
	glVertex3d(P4.x, P4.y, P4.z);
	glEnd();
	glLineWidth(1);
}   

