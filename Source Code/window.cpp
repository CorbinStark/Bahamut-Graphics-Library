///////////////////////////////////////////////////////////////////////////
// FILE:                       window.cpp                                //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Corbin Stark                                       //
//                                                                       //
// Permission is hereby granted, free of charge, to any person obtaining //
// a copy of this software and associated documentation files (the       //
// "Software"), to deal in the Software without restriction, including   //
// without limitation the rights to use, copy, modify, merge, publish,   //
// distribute, sublicense, and/or sell copies of the Software, and to    //
// permit persons to whom the Software is furnished to do so, subject to //
// the following conditions:                                             //
//                                                                       //
// The above copyright notice and this permission notice shall be        //
// included in all copies or substantial portions of the Software.       //
//                                                                       //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       //
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.//
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  //
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  //
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     //
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                //
///////////////////////////////////////////////////////////////////////////

#include "window.h"
#include "render2D.h"
#include "render3D.h"
#include "audio.h"
#include <iostream>
#include <thread>

struct BMTWindow {
	GLFWwindow* glfw_window;
	int x;
	int y;
	int width;
	int height;
	int virtual_width;
	int virtual_height;
	double mousex;
	double mousey;

	bool hidden;
	bool locked;

	double targetFPS;

	const char* title;
	int keys[MAX_KEYS];
	int buttons[MAX_BUTTONS];

	double currentTime, previousTime;
	double updateTime, drawTime;
	double frameTime = 0.0;
	double targetTime = 0.0;

	void(*BMTKeyCallback)(int key, int action);
	void(*BMTMouseCallback)(double mousex, double mousey, int button, int action);
	void(*BMTResizeCallback)(int width, int height);
};
INTERNAL BMTWindow bmt_win;

INTERNAL
void rebuildState() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

//TODO: implement the GUI into this engine.
INTERNAL
void keycallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	bmt_win.keys[key] = action;
	if(bmt_win.BMTKeyCallback != NULL)
		bmt_win.BMTKeyCallback(key, action);
	//Panel::key_callback_func(key, action);
}

INTERNAL
void cursorPosCallback(GLFWwindow* win, double xPos, double yPos) {
	bmt_win.mousex = xPos;
	bmt_win.mousey = yPos;
	//Panel::mouse_pos_callback_func(xPos, yPos);
}

INTERNAL
void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
	bmt_win.buttons[button] = action;
	if (bmt_win.BMTMouseCallback != NULL)
		bmt_win.BMTMouseCallback(get_mouse_pos().x, get_mouse_pos().y, button, action);
	//Panel::mouse_callback_func(button, action, bmt_win.mousex, bmt_win.mousey);
}

INTERNAL
void resizeCallback(GLFWwindow* win, int width, int height) {
	if (bmt_win.BMTResizeCallback != NULL)
		bmt_win.BMTResizeCallback(width, height);
	set_window_size(width, height);
}

void init_window(int width, int height, const char* title, bool fullscreen, bool resizable, bool primary_monitor) {
	bmt_win.width = width;
	bmt_win.height = height;

	bmt_win.virtual_width = width;
	bmt_win.virtual_height = height;

	//INIT GLFW
	if (!glfwInit()) {
		BMT_LOG(MINOR_ERROR, "GLFW could not initialize");
	}
	else {
		BMT_LOG(INFO, "GLFW has initialized");
	}

	for (int i = 0; i < MAX_KEYS; ++i)
		bmt_win.keys[i] = -1;
	for (int i = 0; i < MAX_BUTTONS; ++i)
		bmt_win.buttons[i] = -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	//END INIT GLFW

	glfwWindowHint(GLFW_RESIZABLE, resizable);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	if (fullscreen) {
		glfwWindowHint(GLFW_VISIBLE, false);
		glfwWindowHint(GLFW_DECORATED, false);
		bmt_win.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!bmt_win.glfw_window) {
			BMT_LOG(MINOR_ERROR, "Windowed Window failed to be created");
			glfwTerminate();
		}
		bmt_win.x = bmt_win.y = 0;

		set_window_size(
			glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
			glfwGetVideoMode(glfwGetPrimaryMonitor())->height
		);

		glfwSetWindowPos(bmt_win.glfw_window, 0, 0);
		if(!primary_monitor) 
			glfwSetWindowPos(bmt_win.glfw_window, glfwGetVideoMode(glfwGetPrimaryMonitor())->width, 0);
	}
	else {
		glfwWindowHint(GLFW_VISIBLE, false);
		bmt_win.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!bmt_win.glfw_window) {
			BMT_LOG(MINOR_ERROR, "Windowed Window failed to be created");
			glfwTerminate();
		}
		bmt_win.x = bmt_win.y = 0;

		float scrWidth = (float)glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
		float scrHeight = (float)glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

		glfwSetWindowPos(bmt_win.glfw_window, (int)((scrWidth / 2) - (width / 2)), (int)((scrHeight / 2) - (height / 2)));
		if (!primary_monitor)
			glfwSetWindowPos(bmt_win.glfw_window, (int)(glfwGetVideoMode(glfwGetPrimaryMonitor())->width + (scrWidth / 2) - (width / 2)),
			(int)((scrHeight / 2) - (height / 2))
		);
	}

	//INIT GLEW
	glfwMakeContextCurrent(bmt_win.glfw_window);
	glfwSwapInterval(0);
	glfwShowWindow(bmt_win.glfw_window);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		BMT_LOG(MINOR_ERROR, "%s", glewGetErrorString(err));
		BMT_LOG(MINOR_ERROR, "GLEW could not initialize");
	}
	else {
		BMT_LOG(INFO, "GLEW has initialized");
	}
	rebuildState();

	BMT_LOG(INFO, "OpenGL Version: %s", glGetString(GL_VERSION));
	BMT_LOG(INFO, "GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	BMT_LOG(INFO, "OpenGL Vendor: %s", glGetString(GL_VENDOR));
	BMT_LOG(INFO, "Graphics Card: %s", glGetString(GL_RENDERER));

	glfwSetKeyCallback(bmt_win.glfw_window, keycallback);
	glfwSetWindowSizeCallback(bmt_win.glfw_window, resizeCallback);
	glfwSetMouseButtonCallback(bmt_win.glfw_window, mouseButtonCallback);
	glfwSetCursorPosCallback(bmt_win.glfw_window, cursorPosCallback);

	//END INIT GLEW

	init2D(0, 0, width, height);
	init3D();
}

void set_window_pos(int x, int y) {
	bmt_win.x = x;
	bmt_win.y = y;
	glfwSetWindowPos(bmt_win.glfw_window, x, y);
}

void set_window_size(int width, int height) {
	if (width < 1) width = 1;
	if (height < 1) height = 1;
	bmt_win.width = width;
	bmt_win.height = height;
	glfwSetWindowSize(bmt_win.glfw_window, width, height);
	set_2D_render_viewport(0, 0, width, height, bmt_win.virtual_width, bmt_win.virtual_height);
	set_3D_render_viewport(width, height);
}

void set_clear_color(float r, float g, float b, float a) {
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void set_clear_color(vec4 color) {
	set_clear_color(color.x, color.y, color.z, color.w);
}

void begin_drawing() {
	bmt_win.currentTime = glfwGetTime();
	bmt_win.updateTime = bmt_win.currentTime - bmt_win.previousTime;
	bmt_win.previousTime = bmt_win.currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void end_drawing() {
	for (int i = 0; i < MAX_KEYS; ++i) {
		bmt_win.keys[i] = -1;
	}
	for (int i = 0; i < MAX_BUTTONS; ++i) {
		bmt_win.buttons[i] = -1;
	}

	glfwSwapBuffers(bmt_win.glfw_window);
	glfwPollEvents();

	bmt_win.currentTime = glfwGetTime();
	bmt_win.drawTime = bmt_win.currentTime - bmt_win.previousTime;
	bmt_win.previousTime = bmt_win.currentTime;

	bmt_win.frameTime = bmt_win.updateTime + bmt_win.drawTime;

	// Wait for some milliseconds...
	if (bmt_win.frameTime < bmt_win.targetTime)
	{
		double prevTime = glfwGetTime();
		double nextTime = 0.0;

#if defined(_BUSY_WAIT)
		// Busy wait loop
		while ((bmt_win.nextTime - bmt_win.prevTime) < ((bmt_win.targetTime - bmt_win.frameTime)*1000.0f) / 1000.0f) bmt_win.nextTime = glfwGetTime();
#elif defined(_WIN32) || defined(_WIN64)
		Sleep((DWORD)((bmt_win.targetTime - bmt_win.frameTime)*1000.0f));
#elif defined(__LINUX__)
		usleep((bmt_win.targetTime - bmt_win.frameTime)*1000.0f);
#elif defined(__APPLE__)
		usleep((bmt_win.targetTime - bmt_win.frameTime)*1000.0f);
#endif

		bmt_win.currentTime = glfwGetTime();
		double extraTime = bmt_win.currentTime - bmt_win.previousTime;
		bmt_win.previousTime = bmt_win.currentTime;

		bmt_win.frameTime += extraTime;
	}
	static int framecount = 0;
	framecount++;
	if (framecount > 150) {
		std::cout << "FPS:" << (int)(1.0f / (float)bmt_win.frameTime) << std::endl;
		framecount = 0;
	}
}

double get_elapsed_time() {
	return glfwGetTime();
}

void set_FPS_cap(double FPS) {
	if (FPS < 1) bmt_win.targetTime = 0.0;
	else bmt_win.targetTime = 1.0 / FPS;
}

bool is_window_closed() {
	return glfwWindowShouldClose(bmt_win.glfw_window) == 1;
}

void set_key_callback(void(*keyCallback)(int key, int action)) {
	bmt_win.BMTKeyCallback = keyCallback;
}

void set_mouse_callback(void(*mouseCallback)(double mousex, double mousey, int button, int action)) {
	bmt_win.BMTMouseCallback = mouseCallback;
}

void set_window_resize_callback(void(*resizecallback)(int width, int height)) {
	bmt_win.BMTResizeCallback = resizecallback;
}

bool is_key_pressed(unsigned int keycode) {
	if (bmt_win.keys[keycode] == GLFW_PRESS)
		return true;
	return false;
}

bool is_key_released(unsigned int keycode) {
	if (bmt_win.keys[keycode] == GLFW_RELEASE) {
		return true;
	}
	return false;
}

bool is_button_pressed(unsigned int button) {
	if (bmt_win.buttons[button] == GLFW_PRESS) {
		return true;
	}
	return false;
}

bool is_button_released(unsigned int button) {
	if (bmt_win.buttons[button] == GLFW_RELEASE) {
		return true;
	}
	return false;
}

bool is_key_down(unsigned int keycode) {
	if (glfwGetKey(bmt_win.glfw_window, keycode) == 1) {
		return true;
	}
	return false;
}

bool is_button_down(unsigned int button) {
	if (glfwGetMouseButton(bmt_win.glfw_window, button) == 1) {
		return true;
	}
	return false;
}

bool is_key_up(unsigned int keycode) {
	if (glfwGetKey(bmt_win.glfw_window, keycode) == 0) {
		return true;
	}
	return false;
}

bool is_button_up(unsigned int button) {
	if (glfwGetMouseButton(bmt_win.glfw_window, button) == 0) {
		return true;
	}
	return false;
}

void set_vsync(bool vsync) {
	if (vsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void get_mouse_pos(double* mousex, double* mousey) {
	*mousex = bmt_win.mousex;
	*mousey = bmt_win.mousey;
	*mousex *= (float)bmt_win.virtual_width / (float)bmt_win.width;
	*mousey *= (float)bmt_win.virtual_height / (float)bmt_win.height;
}

vec2 get_mouse_pos() {
	vec2 mouse_pos = V2((float)bmt_win.mousex, (float)bmt_win.mousey);
	vec2 scale = V2((float)(bmt_win.virtual_width) / (float)(bmt_win.width), (float)(bmt_win.virtual_height) / (float)(bmt_win.height));
	mouse_pos = mouse_pos * scale;
	return mouse_pos;
}

void dispose_window() {
	glfwSetWindowShouldClose(bmt_win.glfw_window, true);
	glfwDestroyWindow(bmt_win.glfw_window);
	glfwDefaultWindowHints();
	glfwTerminate();
	dispose2D();
}

void set_mouse_locked(bool locked) {
	bmt_win.locked = locked;
}

void set_virtual_size(int v_width, int v_height) {
	bmt_win.virtual_width = v_width;
	bmt_win.virtual_height = v_height;
}

int get_virtual_width() {
	return bmt_win.virtual_width;
}

int get_virtual_height() {
	return bmt_win.virtual_height;
}

vec2 get_virtual_size() {
	return V2((float)bmt_win.virtual_width, (float)bmt_win.virtual_height);
}

int get_window_width() {
	int width;
	glfwGetWindowSize(bmt_win.glfw_window, &width, 0);
	return width;
}

int get_window_height() {
	int height;
	glfwGetWindowSize(bmt_win.glfw_window, 0, &height);
	return height;
}

void set_mouse_hidden(bool hidden) {
	bmt_win.hidden = hidden;
	if (hidden)
		glfwSetInputMode(bmt_win.glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else
		glfwSetInputMode(bmt_win.glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}