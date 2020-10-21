#pragma once

#include <Windows.h>
#include <atomic>

struct Message
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

void setHwnd(HWND window);

void add_message(Message msg);

void start_thread();
void start_msg_thread();

void join_render_thread();
void join_msg_thread();

void stop_all_threads();

class OpenGL
{
	HWND g_hWnd;
	HDC g_hDC;
	HGLRC g_hRC;
	std::atomic_int width, height;
	std::atomic_int tmp_width, tmp_height;
	
	
	double camDist;
	double camX, camY, camZ;
	double camNz;
	double fi1, fi2;

	int mouseX, mouseY;

	std::atomic_bool resize_pending;

public:

	OpenGL();
	~OpenGL();

	void setHWND(HWND window);

	void drawPlane();

	void wheelEvent(float delta);

	void mouseMovie(int mX, int mY);

	void SetUpLight();

	void SetUpMaterial();

	void SetUpCamera();

	void DrawCube(float x, float y, float z);


	void DrawCube();
	void DrawAxes();

	void render(double);

	void resize(int w, int h);
	void try_to_resize(int w, int h);

	void init(void);

};



