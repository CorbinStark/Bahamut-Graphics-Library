#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdlib.h>
#include <vector>
#include "maths.h"

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

void initWindow(int width, int height, const char* title, bool fullscreen, bool resizable, bool primary_monitor);

void setWindowPos(int x, int y);
void setWindowSize(int width, int height);
void beginDrawing();
void endDrawing();
bool isWindowClosed();

void setClearColor(float r, float g, float b, float a);
void setClearColor(vec4f& color);
void setWindowResizeCallback(void(*resizecallback)(int width, int height));

bool isKeyPressed(unsigned int keycode);
bool isKeyReleased(unsigned int keycode);
bool isButtonPressed(unsigned int button);
bool isButtonReleased(unsigned int button);
bool isKeyDown(unsigned int keycode);
bool isButtonDown(unsigned int button);
bool isKeyUp(unsigned int keycode);
bool isButtonUp(unsigned int button);

double getElapsedTime();

void getMousePos(double* mousex, double* mousey);
vec2f getMousePos();
void disposeWindow();

void setFPSCap(double FPS);

void setMouseLocked(bool locked);
void setMouseHidden(bool hidden);
void setVSync(bool vsync);

int getWindowWidth();
int getWindowHeight();