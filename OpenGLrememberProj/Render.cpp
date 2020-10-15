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

#include <tuple>

#include "Vertex.h"

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

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
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
}

size_t size = 0;
size_t convex_circle_count_points = 0;
point_t get_text_point(const point_t& prev);

void append_convex_circle(vertexes& vertexes, const points& all_text_points) {
	convex_circle_count_points = 0;
	point_t first_point = vertexes[0].figure_bot; //point with coor 4, -2
	point_t second_point = vertexes[2].figure_bot; //point with coor 4, 4
	point_t center = { (first_point.x + second_point.x) / 2, (first_point.y + second_point.y) / 2 };
	double radius = sqrt(pow(first_point.x - second_point.x, 2) + pow(first_point.y - second_point.y, 2)) / 2;


	point_t text_center_bot = { vertexes[0].texture_extreme_bot.x, (vertexes[0].texture_extreme_bot.y - vertexes[2].texture_extreme_bot.y) / 2 + vertexes[2].texture_extreme_bot.y};
	point_t text_center_top = { vertexes[0].texture_extreme_top.x, (vertexes[0].texture_extreme_top.y - vertexes[2].texture_extreme_top.y) / 2 + vertexes[2].texture_extreme_top.y };
	double text_bot_radius = sqrt(pow(vertexes[0].texture_extreme_bot.x - vertexes[2].texture_extreme_bot.x, 2) + pow(vertexes[0].texture_extreme_bot.y - vertexes[2].texture_extreme_bot.y, 2)) / 2;
	double text_top_radius = sqrt(pow(vertexes[0].texture_extreme_top.x - vertexes[2].texture_extreme_top.x, 2) + pow(vertexes[0].texture_extreme_top.y - vertexes[2].texture_extreme_top.y, 2)) / 2;

	vertexes.emplace_back(center, text_center_bot, text_center_top);//center vertex
	vertexes.emplace_back(first_point, vertexes[0].texture_extreme_bot, vertexes[0].texture_extreme_top, vertexes[0].texture_side_bot, vertexes[0].texture_side_top);//first point
	convex_circle_count_points += 2;

	double dx = std::abs((all_text_points[17].x - all_text_points[19].x) / 181);
	for (double i = -90; i < 91; ++i) {
		double iter = i * M_PI / 180;
		int j = i + 90;
		vertexes.emplace_back(
			point_t(radius * cos(iter) + center.x, radius * sin(iter) + center.y),
			point_t(text_bot_radius * cos(iter) + text_center_bot.x, text_center_bot.y - text_bot_radius * sin(iter)),
			point_t(text_center_top.x - text_top_radius * cos(iter), text_center_top.y - text_top_radius * sin(iter)),
			point_t(all_text_points[19].x - dx * j, all_text_points[19].y),
			point_t(all_text_points[20].x - dx * j, all_text_points[20].y)
		);
		++convex_circle_count_points;
	}
	vertexes.emplace_back(second_point, vertexes[2].texture_extreme_bot, vertexes[2].texture_extreme_top, vertexes[2].texture_side_bot, vertexes[2].texture_side_top);
	++convex_circle_count_points;
}

std::tuple<point_t, double> get_center_and_radius(const point_t& first_point, const point_t& second_point, const point_t& third_point) {
	double A = second_point.x - first_point.x;
	double B = second_point.y - first_point.y;
	double C = third_point.x - first_point.x;
	double D = third_point.y - first_point.y;
	double E = A * (first_point.x + second_point.x) + B * (first_point.y + second_point.y);
	double F = C * (first_point.x + third_point.x) + D * (first_point.y + third_point.y);
	double G = 2 * (A * (third_point.y - second_point.y) - B * (third_point.x - second_point.x));
	point_t c = { (D * E - B * F) / G, (A * F - C * E) / G };
	double R = sqrt(pow(first_point.x - c.x, 2) + pow(first_point.y - c.y, 2));
	return { c, R };
}

void append_nested_circle(vertexes& vertexes, const points& all_text_points) {
	point_t point_M = { 0, -4 }; // point M
	auto tmp = get_center_and_radius(vertexes[13].figure_bot, vertexes[9].figure_bot, point_M);
	point_t figure_center = std::get<0>(tmp);
	double figure_radius = std::get<1>(tmp);
	
	point_t text_point_M = { 255, 347 };//texture point M
	tmp = get_center_and_radius(all_text_points[5], all_text_points[8], text_point_M);
	point_t text_bot_center = std::get<0>(tmp);
	double text_bot_radius = std::get<1>(tmp);

	text_point_M = { 797, 736 };//texture point M
	tmp = get_center_and_radius(all_text_points[32], all_text_points [31], text_point_M);
	point_t text_top_center = std::get<0>(tmp);
	double text_top_radius = std::get<1>(tmp);


	vertexes.emplace_back(vertexes[6].figure_bot, vertexes[6].texture_extreme_bot, vertexes[6].texture_extreme_top, vertexes[6].texture_side_bot, vertexes[6].texture_side_top);//point with coor -2, -1
	vertexes.emplace_back(vertexes[13].figure_bot, vertexes[13].texture_extreme_bot, vertexes[13].texture_extreme_top, vertexes[13].texture_side_bot, vertexes[13].texture_side_top);//point with coor -3, -4
	double dx = std::abs((all_text_points[7].x - all_text_points[5].x) / 68);
	double dy = std::abs((all_text_points[7].y - all_text_points[5].y) / 68);
	double qx0 = acos((vertexes[13].texture_extreme_top.x - text_top_center.x) / text_top_radius);
	double qx1 = acos((vertexes[9].texture_extreme_top.x - text_top_center.x) / text_top_radius);
	double dxq = (qx1 - qx0) / 68;
	double qy0 = 73;
	double qy1 = 145;
	double dyq = (qy1 - qy0) / 68;
	qx0 += dxq;
	for (int i = 105, j = 0; i > 37; --i, ++j, qx0 += dxq, qy0 += dyq) {
		double iter = i * M_PI / 180;
		double y_iter = qy0 * M_PI / 180;
	vertexes.emplace_back(
			point_t(figure_radius * cos(iter) + figure_center.x, figure_radius * sin(iter) + figure_center.y),
			point_t(text_bot_radius * cos(iter) + text_bot_center.x, text_bot_center.y - text_bot_radius * sin(iter)),
			point_t(text_top_center.x + text_top_radius * cos(qx0), text_top_center.y - text_top_radius * sin(y_iter)),
			point_t(all_text_points[5].x + j * dx, all_text_points[5].y + j * dy),
			point_t(all_text_points[4].x + j * dx, all_text_points[4].y + j * dy)
		);
	}
	vertexes.emplace_back(vertexes[9].figure_bot, vertexes[9].texture_extreme_bot, vertexes[9].texture_extreme_top, vertexes[9].texture_side_bot, vertexes[9].texture_side_top);//point with coor 3, 6
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

vertexes filling_vertexes(const points& fig_points, const points& all_text_points, double degrees) {
	vertexes vertexes;
	vertexes.emplace_back(fig_points[0], all_text_points[14], all_text_points[26], all_text_points[19], all_text_points[20]);
	vertexes.emplace_back(fig_points[1], all_text_points[14], all_text_points[26], all_text_points[16], all_text_points[15]);
	vertexes.emplace_back(fig_points[2], all_text_points[17], all_text_points[27], all_text_points[17], all_text_points[18]);
	vertexes.emplace_back(fig_points[3], all_text_points[12], all_text_points[33], all_text_points[14], all_text_points[13]);
	vertexes.emplace_back(fig_points[4], all_text_points[12], all_text_points[33], all_text_points[8], all_text_points[9]);
	vertexes.emplace_back(fig_points[5], all_text_points[21], all_text_points[28], all_text_points[21], all_text_points[22]);
	vertexes.emplace_back(fig_points[6], all_text_points[25], all_text_points[29], all_text_points[23], all_text_points[24]);
	vertexes.emplace_back(fig_points[7], all_text_points[25], all_text_points[29], all_text_points[1], all_text_points[0]);
	vertexes.emplace_back(fig_points[8], all_text_points[12], all_text_points[33], all_text_points[14], all_text_points[13]);
	vertexes.emplace_back(fig_points[9], all_text_points[8], all_text_points[32], all_text_points[7], all_text_points[6]);
	vertexes.emplace_back(fig_points[10], all_text_points[8], all_text_points[32], all_text_points[10], all_text_points[11]);
	vertexes.emplace_back(fig_points[11], all_text_points[25], all_text_points[29], all_text_points[23], all_text_points[24]);
	vertexes.emplace_back(fig_points[12], all_text_points[3], all_text_points[30], all_text_points[3], all_text_points[2]);
	vertexes.emplace_back(fig_points[13], all_text_points[5], all_text_points[31], all_text_points[5], all_text_points[4]);
	size = vertexes.size();
	append_convex_circle(vertexes, all_text_points);
	append_nested_circle(vertexes, all_text_points);
	for (auto& elem : vertexes) {
		elem.figure_top.x = elem.figure_bot.x;
		elem.figure_top.y = elem.figure_bot.y;
		elem.figure_top.z = 1;
		elem.figure_top.rotate_top_point(degrees);
	}
	return vertexes;
}

point_t get_text_point(const point_t& prev) {
	return { prev.x / 1024,  (1024 - prev.y) / 1024,  prev.z };
}

void draw_extreme_panels(const vertexes& vertexes, const double* color) {
	size_t last_convex_index = size + convex_circle_count_points;
	double tmp[3] = { 0, 1, 0 };
	glColor3dv(color);
	vector_t top_normal = { 0, 0, 1 };
	vector_t bot_normal = { 0, 0, -1 };
	glNormal3dv((double*)&bot_normal);
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			if (vertexes[i].figure_bot == vertexes[i + 1].figure_bot) {
				++i;
			}
			auto text = get_text_point(vertexes[i].texture_extreme_bot);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_bot;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();
	glColor3dv(tmp);
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			auto text = get_text_point(vertexes[i].texture_extreme_bot);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_bot;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < vertexes.size(); ++i) {
			auto text = get_text_point(vertexes[i].texture_extreme_bot);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_bot;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();
	glColor3dv(color);
	glNormal3dv((double*)&top_normal);
	if (alphaMode) {
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	double alpha = 0.6;
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			if (vertexes[i].figure_top == vertexes[i + 1].figure_top) {
				++i;
			}
			glColor4d(0, 0, 1, alpha);
			auto text = get_text_point(vertexes[i].texture_extreme_top);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_top;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();	
	glColor3dv(tmp);
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glColor4d(0, 1, 0, alpha);
			auto text = get_text_point(vertexes[i].texture_extreme_top);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_top;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < vertexes.size(); ++i) {
			glColor4d(0, 1, 0, alpha);
			auto text = get_text_point(vertexes[i].texture_extreme_top);
			glTexCoord2d(text.x, text.y);
			auto tmp = vertexes[i].figure_top;
			glVertex3d(tmp.x, tmp.y, tmp.z);
		}
	}
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

double* get_normal(point_t first, point_t second, point_t third) {
	vector_t a = { second.x - first.x, second.y - first.y, second.z - first.z };
	vector_t b = { third.x - first.x, third.y - first.y, third.z - first.z };
	vector_t norm = { a.y * b.z - b.y * a.z, -a.x * b.z + b.x * a.z, a.x * b.y - b.x * a.y };
	double len = sqrt(pow(norm.x, 2) + pow(norm.y, 2) + pow(norm.z, 2));
	return (double*)&vector_t(norm.x / len, norm.y / len, norm.z / len);
}

void draw_one_side_panel(const vertexes& vertexes, const size_t(&val)[2]) {
	glNormal3dv(get_normal(vertexes[val[0]].figure_top, vertexes[val[0]].figure_bot, vertexes[val[1]].figure_bot));
	glTexCoord2dv((double*)&get_text_point(vertexes[val[0]].texture_side_bot));
	glVertex3dv((double*)&vertexes[val[0]].figure_bot);
	glTexCoord2dv((double*)&get_text_point(vertexes[val[0]].texture_side_top));
	glVertex3dv((double*)&vertexes[val[0]].figure_top);
	glTexCoord2dv((double*)&get_text_point(vertexes[val[1]].texture_side_bot));
	glVertex3dv((double*)&vertexes[val[1]].figure_bot);
	glTexCoord2dv((double*)&get_text_point(vertexes[val[1]].texture_side_top));
	glVertex3dv((double*)&vertexes[val[1]].figure_top);
}

void draw_row_side_panels(const vertexes& vertexes, const double(&colors)[2][3]) {
	std::vector<size_t> indexes = { 10, 4, 3, 1, 0, 2, 5, 6, 7, 12, 13 };
	size_t last_convex_index = size + convex_circle_count_points;
	bool color_flag = false;
	for (size_t i = 0; i < indexes.size() - 1; ++i) {
		if (indexes[i] != 0) {
			glColor3dv(get_color(colors, color_flag));
			draw_one_side_panel(vertexes, { indexes[i], indexes[i + 1] });
		} else if (indexes[i] == 0) {
			for (size_t j = size + 1; j < last_convex_index - 1; ++j) {
				glColor3dv(get_color(colors, color_flag));
				draw_one_side_panel(vertexes, { j, j + 1 });
			}
		}
	}
	for (size_t j = last_convex_index + 1; j < vertexes.size() - 1; ++j) {
		glColor3dv(get_color(colors, color_flag));
		draw_one_side_panel(vertexes, { j, j + 1 });
	}
}

void create_one_side_row(vertexes& vertexs, double degrees, double angle) {
	double dz = 1 / degrees;
	for (auto& vertexe : vertexs) {
		delta_t delta;
		delta.texture = point_t((vertexe.texture_side_top - vertexe.texture_side_bot) / degrees);
		delta.figure.z = dz;
		auto tmp = vertexe.figure_bot;
		vertexe = vertex(
			vertexe.figure_top,
			vertexe.texture_extreme_bot,
			vertexe.texture_extreme_top,
			vertexe.texture_side_top,
			vertexe.texture_side_top + delta.texture
		);
		vertexe.figure_top = vertexe.figure_bot;
		vertexe.figure_top.z += dz;
		vertexe.figure_top.rotate_top_point(1);
	}
}

void draw_side_panels(const vertexes& vertexes, double degrees) {
	const double side_colors[][3] = {
		{0.7, 0.2, 0.4},
		{0.1, 0.8, 0.3}
	};
	glBegin(GL_QUAD_STRIP); {
		if (degrees > 1) {
			auto row = vertexes;
			for (auto& vertex : row) {
				vertex.texture_side_top = vertex.texture_side_bot + (vertex.texture_side_top - vertex.texture_side_bot) / degrees;
				vertex.figure_top = vertex.figure_bot;
				vertex.figure_top.z = 1 / degrees;
				vertex.figure_top.rotate_top_point(1);
			}

			draw_row_side_panels(row, side_colors);
			for (double i = 1; i < degrees; ++i) {
				create_one_side_row(row, degrees, i);
				draw_row_side_panels(row, side_colors);
			}
		}
		else {
			draw_row_side_panels(vertexes, side_colors);
		}
	}
	glEnd();
}

void Render(OpenGL *ogl)
{
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
	auto all_texture_points = points({
		{8, 20},//0
		{39, 8},//1
		{53, 155},//2
		{85, 144},//3
		{121, 358},//4
		{153, 348},//5
		{192, 574},//6
		{224, 563},//7
		{357, 416},//8
		{387, 401},//9
		{458, 619},//10
		{488, 603},//11
		{255, 213},//12
		{375, 310},//13
		{390, 280},//14
		{578, 412},//15
		{593, 382},//16
		{390, 76},//17
		{390, 43},//18
		{709, 76},//19
		{709, 43},//20
		{255, 76},//21
		{255, 43},//22
		{72, 76},//23
		{72, 43},//24
		{187, 246},//25
		{661, 670},//26 (294, 226)
		{661, 466},//27 (294, 90)
		{797, 467},//28 (204, 90)
		{864, 636},//29 (159, 202)
		{966, 534},//30 (91, 135)
		{898, 738},//31 (136, 271)
		{697, 803},//32 (272, 316)
		{797, 602}//33 (204, 181)
	});
	auto fig_points = points({
		{4, -2},//0 yellow side circle
		{4, -2},//1 orange side panel
		{4, 4},//2
		{0, 0},//3 orange side panel 
		{0, 0},//4 pink side panel
		{0, 4},//5
		{-2, -1},//6 red side panel
		{-2, -1},//7 blue side panel
		{0, 0},//8
		{3, -6},//9 purple side circle
		{3, -6},//10 pink side panel
		{-2, -1},//11
		{-5, 2},//12
		{-3, -4}//13
	});
	double degrees = 11;
	auto vertexes = filling_vertexes(fig_points, all_texture_points, degrees);
	double extreme_color[] = { 0.2, 0.3, 0.7 };
	glBindTexture(GL_TEXTURE_2D, texId[change_texture]);
	draw_side_panels(vertexes, degrees);
	draw_extreme_panels(vertexes, extreme_color);
	


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
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}