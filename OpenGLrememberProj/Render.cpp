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

bool textureMode = true;
bool lightMode = true;
bool alphaMode = true;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
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
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
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
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
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

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
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
	}

	if (key == 'A') {
		alphaMode = !alphaMode;
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



GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}



size_t size = 0;
size_t convex_circle_count_points = 0;

using point_t = struct s_point {
	s_point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	s_point(double x, double y) {
		this->x = x;
		this->y = y;
		this->z = 0;
	}
	double x;
	double y;
	double z;
};

using vector_t =  struct s_vector {
	s_vector(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	double x;
	double y;
	double z;
};

void append_convex_circle(std::vector<point_t>& panel) {
	convex_circle_count_points = 0;
	point_t first_point = { 4, -2 }; //point with coor 4, -2
	point_t second_point = { 4, 4 }; //point with coor 4, 4
	point_t center = { (first_point.x + second_point.x) / 2, (first_point.y + second_point.y) / 2 };
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

void append_nested_circle(std::vector<point_t>& panel) {
	point_t first_point = { -3, -4 };//point with coor -3, -4
	point_t second_point = { 3, -6 };//point with coor 3, -6
	point_t third_point = { 0, -4 }; // point M
	double A = second_point.x - first_point.x;
	double B = second_point.y - first_point.y;
	double C = third_point.x - first_point.x;
	double D = third_point.y - first_point.y;
	double E = A * (first_point.x + second_point.x) + B * (first_point.y + second_point.y);
	double F = C * (first_point.x + third_point.x) + D * (first_point.y + third_point.y);
	double G = 2 * (A * (third_point.y - second_point.y) - B * (third_point.x - second_point.x));
	point_t c = { (D * E - B * F) / G, (A * F - C * E) / G };
	double R = sqrt(pow(first_point.x - c.x, 2) + pow(first_point.y - c.y, 2));

	panel.emplace_back(-2, -1); //point with coor -2, -1
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

void rotate_top_panel(std::vector<point_t>& panel, double degrees) {
	double radians = degrees * M_PI / 180;
	for (auto& point : panel) {
		double x = point.x, y = point.y;
		double new_x = x * cos(radians) - y * sin(radians);
		double new_y = x * sin(radians) + y * cos(radians);
		point.x = new_x;
		point.y = new_y;
	}
}

std::vector<point_t> filling_vectors(std::vector<point_t>& bottom) {

	append_convex_circle(bottom);
	append_nested_circle(bottom);
	auto top = bottom;
	for (auto& point : top) {
		point.z = 1;
	}
	rotate_top_panel(top, 0);
	return top;
}

point_t get_text_point(const point_t& prev) {
	return { (prev.x + 6) / 12, (prev.y - 6) / -12, 1 };
}

point_t operator*(const point_t& lhs, const point_t& rhs) {
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

void draw_extreme_panels(const std::vector<point_t>& bottom, const std::vector<point_t>& top, const double* color) {
	size_t last_convex_index = size + convex_circle_count_points;
	double tmp[3] = { 0, 1, 0 };
	glColor3dv(color);
	vector_t top_normal = { 0, 0, 1 };
	vector_t bot_normal = { 0, 0, -1 };
	glNormal3dv((double*)&bot_normal);
	glBegin(GL_TRIANGLE_STRIP); {
		for (size_t i = 0; i < size; ++i) {
			glTexCoord3dv((double*)&get_text_point(bottom[i]));
			glVertex3dv((double*)&bottom[i]);
		}
	}
	glEnd();
	glColor3dv(tmp);
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glTexCoord3dv((double*)&get_text_point(bottom[i]));
			glVertex3dv((double*)&bottom[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < bottom.size(); ++i) {
			glTexCoord3dv((double*)&get_text_point(bottom[i]));
			glVertex3dv((double*)&bottom[i]);
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
			glColor4d(0, 0, 1, alpha);
			auto tmp = get_text_point(top[i]) * point_t( 1, -1, 1 );
			glTexCoord3dv((double*)&tmp);
			glVertex3dv((double*)&top[i]);
		}
	}
	glEnd();	
	glColor3dv(tmp);
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = size; i < last_convex_index; ++i) {
			glColor4d(0, 1, 0, alpha);
			auto tmp = get_text_point(top[i]) * point_t(1, -1, 1);
			glTexCoord3dv((double*)&tmp);
			glVertex3dv((double*)&top[i]);
		}
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN); {
		for (size_t i = last_convex_index; i < top.size(); ++i) {
			glColor4d(0, 1, 0, alpha);
			auto tmp = get_text_point(top[i]) * point_t(1, -1, 1);
			glTexCoord3dv((double*)&tmp);
			glVertex3dv((double*)&top[i]);
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

void draw_one_side_panel(const std::vector<point_t>& bottom, const std::vector<point_t>& top, const size_t(&val)[2]) {
	glNormal3dv(get_normal(top[val[0]], bottom[val[0]], bottom[val[1]]));
	glVertex3dv((double*)&bottom[val[0]]);
	glVertex3dv((double*)&top[val[0]]);
	glVertex3dv((double*)&bottom[val[1]]);
	glVertex3dv((double*)&top[val[1]]);
}

void draw_side_panels(const std::vector<point_t>& bottom, const std::vector<point_t>& top, const double(&colors)[2][3]) {
	std::vector<size_t> indexes = { 6, 5, 0, 1, 3, 4, 8, 9 };
	size_t last_convex_index = size + convex_circle_count_points;
	bool color_flag = false;
	glBegin(GL_QUAD_STRIP); {
		for (size_t i = 0; i < indexes.size() - 1; ++i) {
			if (indexes[i] != 0) {
				glColor3dv(get_color(colors, color_flag));
				draw_one_side_panel(bottom, top, { indexes[i], indexes[i + 1] });
			} else if (indexes[i] == 0) {
				for (size_t j = size + 1; j < last_convex_index - 1; ++j) {
					glColor3dv(get_color(colors, color_flag));
					draw_one_side_panel(bottom, top, { j, j + 1 });
				}
			}
		}
		for (size_t j = last_convex_index + 1; j < bottom.size() - 1; ++j) {
			glColor3dv(get_color(colors, color_flag));
			draw_one_side_panel(bottom, top, { j, j + 1 });
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


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//прогать тут
	std::vector<point_t> bottom = {
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
	const double side_colors[][3] = {
		{0.7, 0.2, 0.4},
		{0.1, 0.8, 0.3}
	};
	glBindTexture(GL_TEXTURE_2D, texId);
	draw_side_panels(bottom, top, side_colors);
	draw_extreme_panels(bottom, top, extreme_color);
	


	//Начало рисования квадратика станкина
	/*double A[2] = { -4, -4 };
	double B[2] = { 4, -4 };
	double C[2] = { 4, 4 };
	double D[2] = { -4, 4 };
	double F[2] = { 2, 2 };

	glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);

	glTexCoord2d(0, 0);
	glVertex2dv(A);
	glTexCoord2d(1, 0);
	glVertex2dv(B);
	glTexCoord2d(0.5, 0.5);
	glVertex2dv(F);
	glTexCoord2d(0, 1);
	glVertex2dv(D);
	glTexCoord2d(1, 1);
	glVertex2dv(C);

	glEnd();*/
	//конец рисования квадратика станкина


   //Сообщение вверху экрана

	
	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	                                //(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "A - вкл/выкл прозрачность" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}