
#include <stdio.h>
#include <Math.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "Render.h"
class OpenGL
{
	HWND g_hWnd;
	HDC g_hDC;
	HGLRC g_hRC;
	int width, height;
	
	double camDist;
	double camX, camY, camZ;
	double camNz;
	double fi1, fi2;

	int mouseX, mouseY;

public:

	OpenGL()
	{
		
		camDist = 15;
		camX = 10;
		camY = 10;
		camZ = 10;
		camNz = 1;

		fi1 = -2;
		fi2 = -2;
	}
	~OpenGL()
	{

	}

	void setHWND(HWND window)
	{
		g_hWnd = window;
	}

	void drawPlane()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor4f(0, 0, 0, 0.1);
		glBegin(GL_QUADS);
		glVertex3d(10, 10, 0);
		glVertex3d(10, -10, 0);
		glVertex3d(-10, -10, 0);
		glVertex3d(-10, 10, 0);
		glEnd();
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		//glDisable(GL_TEXTURE_2D);
	}

	void wheelEvent(float delta)
	{
		if (delta<0 && camDist<=1)
			return;
		if (delta>0 && camDist>=100)
			return;

		camDist+=0.01*delta;

		camX = camDist*cos(fi2)*cos(fi1);
		camY = camDist*cos(fi2)*sin(fi1);
		camZ = camDist*sin(fi2);

	}


	void mouseMovie(int mX, int mY)
	{
		int dx = mouseX - mX;
		int dy = mouseY - mY;
		mouseX = mX;
		mouseY = mY;

		fi1 += 0.01*dx;

		fi2 += -0.01*dy;


		camX = camDist*cos(fi2)*cos(fi1);
		camY = camDist*cos(fi2)*sin(fi1);
		camZ = camDist*sin(fi2);

		if (cos(fi2) <= 0)
			camNz = -1;
		else
			camNz = 1;
		


	}

	void SetUpLight()
	{
		glEnable(GL_LIGHTING);

		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat pos[] = { camX, camY, camZ, 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		// характеристики излучаемого света
		// фоновое освещение (рассе€нный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузна€ составл€юща€ света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаема€ составл€юща€ света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}

	void SetUpMaterial()
	{
		GLfloat mamb[] = { 0.2, 0.2, 0.2, 0.0 }; // цвет без лампы
		GLfloat mdif[] = { 0.5, .5, .5, 0.0 }; // цвет лампы
		GLfloat mspec[] = { .7, .7, 7, 0.0 };  //блика
		GLfloat msh = 30;

		glMaterialfv(GL_FRONT, GL_AMBIENT, mamb); //параметриы материала
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mdif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mspec);
		glMaterialf(GL_FRONT, GL_SHININESS, msh);
	}

	void SetUpCamera()
	{
		gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 0, camNz);
	}

	void DrawCube(float x, float y, float z)
	{
		glPushMatrix();
		glTranslatef(x-.5,y-0.5, z-0.5);
		DrawCube();
		glPopMatrix();
		
	}


	void DrawCube()
	{
		glBegin(GL_QUADS);

			glNormal3d(0, 0, -1);
			glVertex3d(0, 0, 0);
			glVertex3d(1, 0, 0);
			glVertex3d(1, 1, 0);
			glVertex3d(0, 1, 0);
			
			glNormal3d(0, 0, 1);
			glVertex3d(0, 0, 1);
			glVertex3d(1, 0, 1);
			glVertex3d(1, 1, 1);
			glVertex3d(0, 1, 1);
			
			glNormal3d(0, -1, 0);
			glVertex3d(0, 0, 0);
			glVertex3d(1, 0, 0);
			glVertex3d(1, 0, 1);
			glVertex3d(0, 0, 1);

			glNormal3d(0, 1, 0);
			glVertex3d(0, 1, 0);
			glVertex3d(1, 1, 0);
			glVertex3d(1, 1, 1);
			glVertex3d(0, 1, 1);
			
			glNormal3d(-1, 0, 0);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 0, 1);
			glVertex3d(0, 1, 1);
			glVertex3d(0, 1, 0);

			glNormal3d(1, 0, 0);
			glVertex3d(1, 0, 0);
			glVertex3d(1, 0, 1);
			glVertex3d(1, 1, 1);
			glVertex3d(1, 1, 0);
			
		glEnd();

		
	}

	void DrawAxes()
	{
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(10, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 10, 0);

		glColor3f(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 10);
		glEnd();

		glColor3f(0.0f, 0.0f, 0.0f);

		glEnable(GL_LIGHTING);
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();


		SetUpLight();
		SetUpCamera();
		SetUpMaterial();

		DrawAxes();		

		glDisable(GL_LIGHTING);
		
		Render();


		//drawPlane();
		SwapBuffers(g_hDC);
	}

	void resize(int w, int h)
	{
		width = w;
		height = h;
		glViewport(0, 0, width, height);	
		
		glMatrixMode(GL_PROJECTION);		
		glLoadIdentity();	
			
		gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 200.0);

		glMatrixMode(GL_MODELVIEW);							
		glLoadIdentity();									
	}

	void init(void)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 16;

		g_hDC = GetDC(g_hWnd);
		GLuint iPixelFormat = ChoosePixelFormat(g_hDC, &pfd);

		if (iPixelFormat != 0)
		{
			PIXELFORMATDESCRIPTOR bestMatch_pfd;
			DescribePixelFormat(g_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

			if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
			{
				return;
			}

			if (SetPixelFormat(g_hDC, iPixelFormat, &pfd) == FALSE)
			{
				DWORD dwErrorCode = GetLastError();
				return;
			}
		}
		else
		{
			DWORD dwErrorCode = GetLastError();
			return;
		}

		g_hRC = wglCreateContext(g_hDC);
		wglMakeCurrent(g_hDC, g_hRC);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glEnable(GL_DEPTH_TEST);
	}

};



