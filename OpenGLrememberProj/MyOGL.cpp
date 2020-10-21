#include "MyOGL.h"
#include <stdio.h>
#include <Math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <mutex>
#include <thread>
#include <deque>
#include <chrono>

#include "Render.h"


OpenGL gl;

HWND wnd;
std::mutex hwnd_mutex;
std::mutex message_mutex;
std::mutex cam_mutex;
std::thread gl_thread;

std::thread msg_thread;

std::deque<Message> msg_deque;

std::atomic_bool bRender;
std::atomic_bool bMsg;

void thread_cycle ();
void message_cycle();
Message get_message();


void setHwnd(HWND window)
{
	std::lock_guard<std::mutex> guard(hwnd_mutex);
	gl.setHWND(window);
}

void start_thread()
{
	bRender = true;
	gl_thread = std::thread(thread_cycle);
}

void start_msg_thread()
{
	bMsg = true;
	msg_thread = std::thread(message_cycle);
}

void add_message(Message msg)
{
	std::lock_guard<std::mutex> guard(message_mutex);
	msg_deque.push_back(msg);
}

void stop_all_threads()
{
	bRender = false;
	bMsg = false;
	gl_thread.join();
	msg_thread.join();
}

Message get_message()
{
	std::lock_guard<std::mutex> guard(message_mutex);
	auto m = msg_deque.front();
	msg_deque.pop_front();
	return m;
}

void join_render_thread()
{
	if (gl_thread.joinable())
		gl_thread.join();
}

void join_msg_thread()
{
	if (msg_thread.joinable())
		msg_thread.join();
}


void thread_cycle ()
{
	
	gl.init();
	bool b_render = true;

	auto end_render = std::chrono::steady_clock::now();
	
	while (bRender)
	{
		auto cur_time = std::chrono::steady_clock::now();
		auto deltatime = cur_time - end_render;
		double delta = 1.0*std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count()/1000000;
		end_render = cur_time;
		gl.render(delta);

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void message_cycle()
{
	while (bMsg)
	{
				
		while (!msg_deque.empty())
		{
			auto m = get_message();
			switch (m.message)
			{
				case WM_MOUSEWHEEL:		
					gl.wheelEvent(GET_WHEEL_DELTA_WPARAM(m.wParam));
					break;	
				case WM_MOUSEMOVE:
					gl.mouseMovie(LOWORD(m.lParam), HIWORD(m.lParam));
					break;
				case WM_SIZE:
					gl.try_to_resize(LOWORD(m.lParam), HIWORD(m.lParam));					 
					break;
				case WM_CLOSE:
					//b_render = false;
					break;									
			}							
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}

}

OpenGL::OpenGL()
{		
		
		camDist = 15;
		camX = 10;
		camY = 10;
		camZ = 10;
		camNz = 1;

		fi1 = -2;
		fi2 = -2;
}
OpenGL::~OpenGL()
{

}

void OpenGL::setHWND(HWND window)
{
	g_hWnd = window;
}



void OpenGL::wheelEvent(float delta)
{
	
	std::lock_guard<std::mutex> guard(cam_mutex);
	if (delta<0 && camDist<=1)
		return;
	if (delta>0 && camDist>=100)
		return;

	camDist=camDist + 0.01*delta;

	camX = camDist*cos(fi2)*cos(fi1);
	camY = camDist*cos(fi2)*sin(fi1);
	camZ = camDist*sin(fi2);

}


void OpenGL::mouseMovie(int mX, int mY)
{
	
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	
	fi1 = fi1 + 0.01*dx;
	fi2= fi2 - 0.01*dy;

	std::lock_guard<std::mutex> guard(cam_mutex);	
	camX = camDist*cos(fi2)*cos(fi1);
	camY = camDist*cos(fi2)*sin(fi1);
	camZ = camDist*sin(fi2);

	if (cos(fi2) <= 0)
		camNz = -1;
	else
		camNz = 1;
}

void OpenGL::SetUpCamera()
{
	std::lock_guard<std::mutex> guard(cam_mutex);
	gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 0, camNz);
}

void OpenGL::DrawAxes()
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


void OpenGL::render(double delta)
{
	
	if (resize_pending)
	{
		resize_pending = false;
		gl.resize(gl.tmp_width,gl.tmp_height);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
		
	SetUpCamera();

	DrawAxes();		

	glDisable(GL_LIGHTING);
		
	
	Render(delta);


	SwapBuffers(g_hDC);
	static auto end_render = std::chrono::steady_clock::now();

}

void OpenGL::try_to_resize(int w, int h)
{
	resize_pending = true;
	tmp_height = h;
	tmp_width = w;
}

void OpenGL::resize(int w, int h)
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

void OpenGL::init(void)
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