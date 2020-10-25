#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

#include "Point.h"
#include "MathFunc.h"

#include <tuple>
#include <chrono>
#include <thread>
#include <array>
#include <sstream>

bool textureMode = true;
bool lightMode = true;
bool alphaMode = false;

bool change_texture = false;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������

//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����

//������ ���������� ����
int mouseX = 0, mouseY = 0;

std::vector<std::vector<point>> coord({
	{
		{0, 0, 3},
		{0, 1, 2},
		{0, 2, 2},
		{0, 3, 1}
	},
	{
		{1, 0, 1},
		{1, 1, 2},
		{1, 2, -2},
		{1, 3, 1}
	},
	{
		{2, 0, 1},
		{2, 1, 2},
		{2, 2, 2},
		{2, 3, 1}
	},
	{
		{3, 0, 1},
		{3, 1, 2},
		{3, 2, 2},
		{3, 3, 1}
	}
});

void find_and_change_point(Ray r, double dy) {
	double delta = 0.5;
	for (auto& v : coord) {
		for (auto& elem : v) {

			double z = elem.z;

			double k = 0, x = 0, y = 0;
			if (r.direction.Z() == 0)
				k = 0;
			else
				k = (z - r.origin.Z()) / r.direction.Z();

			x = k * r.direction.X() + r.origin.X();
			y = k * r.direction.Y() + r.origin.Y();
			point p = { x, y, z };
			if (p.x > elem.x - delta && p.x < elem.x + delta &&
				p.y > elem.y - delta && p.y < elem.y + delta) {
				elem.z += 0.02 * dy;
			}
		}
	}
};

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);
		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (!OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		/*LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);


		double z = 4;

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		obj = { x, y, z };
		std::stringstream ss;
		//ss << "������ x: " << winX << " y: " << viewport[3] -  winY << " z: " << winZ << std::endl;
		ss << "����� � ������� x: " << x << " y: " << y << " z: " << z << std::endl;
		OutputDebugString(ss.str().c_str());*/
		/*POINT curs;
		GetCursorPos(&curs);
		ScreenToClient(ogl->getHwnd(), &curs);
		double x = curs.x, y = curs.y;
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		gluUnProject(winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);*/

		//std::stringstream ss;
		////ss << "������ x: " << winX << " y: " << viewport[3] -  winY << " z: " << winZ << std::endl;
		//ss << "����� � ������� x: " << posX << " y: " << posY << " z: " << posZ << std::endl;
		//OutputDebugString(ss.str().c_str());
		//find_and_change_point({ posX, posY, posZ });

		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);
		find_and_change_point(r, dy);

	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
		alphaMode = false;
	}

	if (key == 'A') {
		alphaMode = !alphaMode;
		textureMode = false;
	}

	if (key == 'C') {
		change_texture = !change_texture;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

GLuint texId[2];
double multi = 2;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	RGBTRIPLE* texarray2;
	char* texCharArray2;
	int texW2, texH2;
	OpenGL::LoadBMP("texture1.bmp", &texW2, &texH2, &texarray2);
	OpenGL::RGBtoChar(texarray2, texW2, texH2, &texCharArray2);

	//���������� �� ��� ��������
	glGenTextures(2, texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);
	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW2, texH2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray2);
	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//�������� ������
	free(texCharArray);
	free(texarray);
	free(texCharArray2);
	free(texarray2);

	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
	coord *= multi;
}

extern std::chrono::steady_clock::time_point end_render;
double t_max = 0;

void draw_main_grid(const std::vector<std::vector<point>>& coord) {
	glLineWidth(3);
	glColor3d(0, 1, 0);
	for (size_t i = 0; i < coord.size(); ++i) {
		glBegin(GL_LINE_STRIP); {
			for (size_t j = 0; j < coord[i].size(); ++j) {
				glVertex3dv((double*)&(coord[i][j]));
			}
		}
		glEnd();
	}
	for (size_t i = 0; i < coord.size(); ++i) {
		glBegin(GL_LINE_STRIP); {
			for (size_t j = 0; j < coord[i].size(); ++j) {
				glVertex3dv((double*)&(coord[j][i]));
			}
		}
		glEnd();
	}
	glPointSize(10);
	glColor3d(1, 0, 0);
	for (size_t i = 0; i < coord.size(); ++i) {
		glBegin(GL_POINTS); {
			for (size_t j = 0; j < coord[i].size(); ++j) {
				glVertex3dv((double*)&(coord[i][j]));
			}
		}
		glEnd();
	}
}

point get_texture_point(const point& fig) {
	return { fig.x / (3 * multi), fig.y / (3 * multi), 0};
}

void draw_two_triangles(const std::array<point, 4>& trinagle_points) {
	glNormal3dv(get_normal(trinagle_points[0], trinagle_points[1], trinagle_points[2]));

	auto text_point = get_texture_point(trinagle_points[0]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[0]);

	text_point = get_texture_point(trinagle_points[1]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[1]);

	text_point = get_texture_point(trinagle_points[2]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[2]);

	glNormal3dv(get_normal(trinagle_points[3], trinagle_points[1], trinagle_points[2], true));
	text_point = get_texture_point(trinagle_points[1]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[1]);

	text_point = get_texture_point(trinagle_points[2]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[2]);

	text_point = get_texture_point(trinagle_points[3]);
	glTexCoord2dv((double*)&text_point);
	glVertex3dv((double*)&trinagle_points[3]);
}

std::array<point, 4> get_triangles_points(std::vector<std::vector<point>> coord, double u, double v, double du, double dv) {
	std::array<point, 4> arr;
	get_new_besie_surface_point(coord, u, v, arr[0]);
	get_new_besie_surface_point(coord, u + du, v, arr[1]);
	get_new_besie_surface_point(coord, u, v + dv, arr[2]);
	get_new_besie_surface_point(coord, u + du, v + dv, arr[3]);
	return arr;
}

void general(double t_max) {
	double h = 0.1;
	double dv = h * 1;
	double du = h * 1;
	t_max = 1.001;
	draw_main_grid(coord);
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	for (double v = 0; v < t_max - dv; v += dv) {
		glBegin(GL_TRIANGLES); {
			for (double u = 0; u < t_max - du; u += du) {
				auto arr = get_triangles_points(coord, u, v, du, dv);
				draw_two_triangles(arr);
			}
		}
		glEnd();
	}

	//point a = { 4, 4, 4 };
	//glPointSize(10);
	//glBegin(GL_POINTS); {
	//	glVertex3dv((double*)&a);
	//	glVertex3dv((double*)&obj);
	//}
	//glEnd();
}

void Render(OpenGL* ogl)
{
	auto cur_time = std::chrono::steady_clock::now();
	auto deltatime = cur_time - end_render;
	double delta_time = 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count() / 1'000;
	end_render = cur_time;
	t_max += delta_time;
	if (t_max > 1.0001) {
		t_max = 0;
	}


	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//���������(
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���
	
	general(t_max);


	//������ ��������� ���������� ��������
	//double A[2] = { -4, -4 };
	//double B[2] = { 4, -4 };
	//double C[2] = { 4, 4 };
	//double D[2] = { -4, 4 };

	//glColor3d(0.6, 0.6, 0.6);
	//glBindTexture(GL_TEXTURE_2D, texId[change_texture]);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex2dv(A);
	//glTexCoord2d(1, 0);
	//glVertex2dv(B);
	//glTexCoord2d(1, 1);
	//glVertex2dv(C);
	//glTexCoord2d(0, 1);
	//glVertex2dv(D);

	//glEnd();
	//����� ��������� ���������� ��������


   //��������� ������ ������


	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
									//(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "A - ���/���� ������������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}