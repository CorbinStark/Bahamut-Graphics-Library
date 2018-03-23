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

INTERNAL GLFWwindow* glfw_window;
INTERNAL int x;
INTERNAL int y;
INTERNAL int width;
INTERNAL int height;
INTERNAL int virtual_width;
INTERNAL int virtual_height;
INTERNAL double mousex;
INTERNAL double mousey;

INTERNAL bool hidden;
INTERNAL bool locked;

INTERNAL double targetFPS;

INTERNAL const char* title;
INTERNAL int keys[MAX_KEYS];
INTERNAL int buttons[MAX_BUTTONS];

INTERNAL double currentTime, previousTime;
INTERNAL double updateTime, drawTime;
INTERNAL double frameTime = 0.0;
INTERNAL double targetTime = 0.0;

INTERNAL void(*BMTKeyCallback)(int key, int action);
INTERNAL void(*BMTMouseCallback)(double mousex, double mousey, int button, int action);
INTERNAL void(*BMTResizeCallback)(int width, int height);

INTERNAL int lastKeyPressed;
INTERNAL int lastButtonPressed;
INTERNAL int lastKeyReleased;
INTERNAL int lastButtonReleased;

INTERNAL
void rebuildState() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

//TODO: implement the GUI into this engine.
INTERNAL
void keycallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	keys[key] = action;
	if (BMTKeyCallback != NULL)
		BMTKeyCallback(key, action);
	//Panel::key_callback_func(key, action);

	if (action == GLFW_PRESS)
		lastKeyPressed = key;
	else if (action == GLFW_RELEASE)
		lastKeyReleased = key;
}

INTERNAL
void char_callback(GLFWwindow* win, unsigned int key) {
	lastKeyPressed = key;
}

INTERNAL
void cursorPosCallback(GLFWwindow* win, double xPos, double yPos) {
	mousex = xPos;
	mousey = yPos;
	//Panel::mouse_pos_callback_func(xPos, yPos);
}

INTERNAL
void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
	buttons[button] = action;
	if (BMTMouseCallback != NULL)
		BMTMouseCallback(get_mouse_pos().x, get_mouse_pos().y, button, action);
	//Panel::mouse_callback_func(button, action, mousex, mousey);

	if (action == GLFW_PRESS)
		lastButtonPressed = button;
	else if (action == GLFW_RELEASE)
		lastButtonReleased = button;
}

INTERNAL
void resizeCallback(GLFWwindow* win, int width, int height) {
	if (BMTResizeCallback != NULL)
		BMTResizeCallback(width, height);
	set_window_size(width, height);
}

void init_window(int window_width, int window_height, const char* title, bool fullscreen, bool resizable, bool primary_monitor) {
	width = window_width;
	height = window_height;

	virtual_width = window_width;
	virtual_height = window_height;

	lastKeyPressed = lastButtonPressed = 0;

	//INIT GLFW
	if (!glfwInit()) {
		BMT_LOG(MINOR_ERROR, "GLFW could not initialize");
	}
	else {
		BMT_LOG(INFO, "GLFW has initialized");
	}

	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = -1;
	for (int i = 0; i < MAX_BUTTONS; ++i)
		buttons[i] = -1;

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
		glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfw_window) {
			BMT_LOG(MINOR_ERROR, "Windowed Window failed to be created");
			glfwTerminate();
		}
		x = y = 0;

		set_window_size(
			glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
			glfwGetVideoMode(glfwGetPrimaryMonitor())->height
		);

		glfwSetWindowPos(glfw_window, 0, 0);
		if (!primary_monitor)
			glfwSetWindowPos(glfw_window, glfwGetVideoMode(glfwGetPrimaryMonitor())->width, 0);
	}
	else {
		glfwWindowHint(GLFW_VISIBLE, false);
		glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!glfw_window) {
			BMT_LOG(MINOR_ERROR, "Windowed Window failed to be created");
			glfwTerminate();
		}
		x = y = 0;

		float scrWidth = (float)glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
		float scrHeight = (float)glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

		glfwSetWindowPos(glfw_window, (int)((scrWidth / 2) - (width / 2)), (int)((scrHeight / 2) - (height / 2)));
		if (!primary_monitor)
			glfwSetWindowPos(glfw_window, (int)(glfwGetVideoMode(glfwGetPrimaryMonitor())->width + (scrWidth / 2) - (width / 2)),
			(int)((scrHeight / 2) - (height / 2))
			);
	}

	//INIT GLEW
	glfwMakeContextCurrent(glfw_window);
	glfwSwapInterval(0);
	glfwShowWindow(glfw_window);

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

	glfwSetKeyCallback(glfw_window, keycallback);
	glfwSetWindowSizeCallback(glfw_window, resizeCallback);
	glfwSetMouseButtonCallback(glfw_window, mouseButtonCallback);
	glfwSetCursorPosCallback(glfw_window, cursorPosCallback);
	glfwSetCharCallback(glfw_window, char_callback);

	//END INIT GLEW

	init2D(0, 0, width, height);
	init3D();
}

void set_window_pos(int window_x, int window_y) {
	x = window_x;
	y = window_y;
	glfwSetWindowPos(glfw_window, x, y);
}

void set_window_size(int window_width, int window_height) {
	if (window_width < 1) window_width = 1;
	if (window_height < 1) window_height = 1;
	width = window_width;
	height = window_height;
	glfwSetWindowSize(glfw_window, width, height);
	set_2D_render_viewport(0, 0, width, height, virtual_width, virtual_height);
	set_3D_render_viewport(width, height);
}

void set_clear_color(float r, float g, float b, float a) {
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void set_clear_color(vec4 color) {
	set_clear_color(color.x, color.y, color.z, color.w);
}

void begin_drawing() {
	currentTime = glfwGetTime();
	updateTime = currentTime - previousTime;
	previousTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void end_drawing() {
	for (int i = 0; i < MAX_KEYS; ++i) {
		keys[i] = -1;
	}
	for (int i = 0; i < MAX_BUTTONS; ++i) {
		buttons[i] = -1;
	}
	lastKeyPressed = 0;
	lastButtonPressed = 0;
	lastKeyReleased = 0;
	lastButtonReleased = 0;

	glfwSwapBuffers(glfw_window);
	glfwPollEvents();

	currentTime = glfwGetTime();
	drawTime = currentTime - previousTime;
	previousTime = currentTime;

	frameTime = updateTime + drawTime;

	// Wait for some milliseconds...
	if (frameTime < targetTime)
	{
		double prevTime = glfwGetTime();
		double nextTime = 0.0;

#if defined(_BUSY_WAIT)
		// Busy wait loop
		while ((nextTime - prevTime) < ((targetTime - frameTime)*1000.0f) / 1000.0f) nextTime = glfwGetTime();
#elif defined(_WIN32) || defined(_WIN64)
		Sleep((DWORD)((targetTime - frameTime)*1000.0f));
#elif defined(__LINUX__)
		usleep((targetTime - frameTime)*1000.0f);
#elif defined(__APPLE__)
		usleep((targetTime - frameTime)*1000.0f);
#endif

		currentTime = glfwGetTime();
		double extraTime = currentTime - previousTime;
		previousTime = currentTime;

		frameTime += extraTime;
	}
	static int framecount = 0;
	framecount++;
	if (framecount > 150) {
		std::cout << "FPS:" << (int)(1.0f / (float)frameTime) << std::endl;
		framecount = 0;
	}
}

double get_elapsed_time() {
	return glfwGetTime();
}

void set_FPS_cap(double FPS) {
	if (FPS < 1) targetTime = 0.0;
	else targetTime = 1.0 / FPS;
}

bool is_window_closed() {
	return glfwWindowShouldClose(glfw_window) == 1;
}

void set_key_callback(void(*keyCallback)(int key, int action)) {
	BMTKeyCallback = keyCallback;
}

void set_mouse_callback(void(*mouseCallback)(double mousex, double mousey, int button, int action)) {
	BMTMouseCallback = mouseCallback;
}

void set_window_resize_callback(void(*resizecallback)(int width, int height)) {
	BMTResizeCallback = resizecallback;
}

int get_key_pressed() {
	return lastKeyPressed;
}

int get_button_pressed() {
	return lastButtonPressed;
}

int get_key_released() {
	return lastKeyReleased;
}

int get_button_released() {
	return lastButtonReleased;
}

bool is_key_pressed(unsigned int keycode) {
	if (keys[keycode] == GLFW_PRESS)
		return true;
	return false;
}

bool is_key_released(unsigned int keycode) {
	if (keys[keycode] == GLFW_RELEASE) {
		return true;
	}
	return false;
}

bool is_button_pressed(unsigned int button) {
	if (buttons[button] == GLFW_PRESS) {
		return true;
	}
	return false;
}

bool is_button_released(unsigned int button) {
	if (buttons[button] == GLFW_RELEASE) {
		return true;
	}
	return false;
}

bool is_key_down(unsigned int keycode) {
	if (glfwGetKey(glfw_window, keycode) == 1) {
		return true;
	}
	return false;
}

bool is_button_down(unsigned int button) {
	if (glfwGetMouseButton(glfw_window, button) == 1) {
		return true;
	}
	return false;
}

bool is_key_up(unsigned int keycode) {
	if (glfwGetKey(glfw_window, keycode) == 0) {
		return true;
	}
	return false;
}

bool is_button_up(unsigned int button) {
	if (glfwGetMouseButton(glfw_window, button) == 0) {
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

void get_mouse_pos(double* mousexPtr, double* mouseyPtr) {
	*mousexPtr = mousex;
	*mouseyPtr = mousey;
	*mousexPtr *= (float)virtual_width / (float)width;
	*mouseyPtr *= (float)virtual_height / (float)height;
}

vec2 get_mouse_pos() {
	vec2 mouse_pos = V2((float)mousex, (float)mousey);
	vec2 scale = V2((float)(virtual_width) / (float)(width), (float)(virtual_height) / (float)(height));
	mouse_pos = mouse_pos * scale;
	return mouse_pos;
}

void dispose_window() {
	glfwSetWindowShouldClose(glfw_window, true);
	glfwDestroyWindow(glfw_window);
	glfwDefaultWindowHints();
	glfwTerminate();
	dispose2D();
}

void set_mouse_locked(bool mouse_locked) {
	locked = mouse_locked;
}

void set_virtual_size(int v_width, int v_height) {
	virtual_width = v_width;
	virtual_height = v_height;
}

int get_virtual_width() {
	return virtual_width;
}

int get_virtual_height() {
	return virtual_height;
}

vec2 get_virtual_size() {
	return V2((float)virtual_width, (float)virtual_height);
}

int get_window_width() {
	int win_width;
	glfwGetWindowSize(glfw_window, &win_width, 0);
	return win_width;
}

int get_window_height() {
	int win_height;
	glfwGetWindowSize(glfw_window, 0, &win_height);
	return win_height;
}

void set_mouse_hidden(bool mouse_hidden) {
	hidden = mouse_hidden;
	if (hidden)
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else
		glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}